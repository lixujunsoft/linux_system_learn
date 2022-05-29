#include <sys/mman.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/sem.h>
#include <signal.h>

pthread_t ntid;

// 只有两个SIGKILL 和SIGSTOP不能被捕获,阻塞,忽略.
void handler(int num)
{
    if (num == SIGUSR1) {
        printf("handler_%s\n", "SIGUSR1");
    } else if (num == SIGCHLD) {
        printf("handler_%s\n", "SIGCHLD");
    }
    return;
}

void *pthread_handler(void *arg)
{
    sleep(15);
    system("ls -al");
    printf("after system\n");
}

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int sem_p(int sem_id)
{
    int ret = 0;
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = SEM_UNDO;
    ret = semop(sem_id, &sem_buf, 1);
    if (ret == -1 && errno != EINTR) {
        perror("Sem P operation");
        exit(1);
    } else if (ret == -1 && errno == EINTR) {
        ret = semop(sem_id, &sem_buf, 1);
    }
    return ret;
}

int sem_v(int sem_id)
{
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;
    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("Sem V operation");
        exit(1);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, handler);
    signal(SIGCHLD, handler);

    const char *name = "./a.c";
    union semun un;
    union semun myun;
    myun.val = 10;

    // pthread_create(&ntid, NULL, pthread_handler, NULL);

    key_t key = ftok(name, 0);
    if (key == -1) {
        perror("ftok error");
        return -1;
    }

    int semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (-1 == semid) {
        if (errno != EEXIST) {
            printf("semget error: %s\n", strerror(errno));
            return -1;
        }
        semid = semget(key, 0, 0666);
        printf("semget get succ\n");
    } else {
        printf("semget create succ\n");
        un.val = 1;
        if (semctl(semid, 0, SETVAL, un) == -1) {
            printf("semctl error: %s\n", strerror(errno));
            return -1;
        }
    }

    printf("semid = %d, semvalue = %d\n", semid, semctl(semid, 0, SETVAL, myun));
    printf("semid = %d, semvalue = %d\n", semid, semctl(semid, 0, GETVAL, 0));
    printf("pid = %d\n", getpid());
    while (1)
    {
        sem_p(semid);
        printf("hello world\n");
        printf("semid = %d, semvalue = %d\n", semid, semctl(semid, 0, GETVAL, 0));
        sleep(2);
    }
    return 0;
}