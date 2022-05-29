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

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int sem_p(int sem_id)
{
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;
    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("Sem P operation");
        exit(1);
    }
    return 0;
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
    const char *name = "./a.c";
    union semun un;

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

    printf("semid = %d, semvalue = %d\n", semid, semctl(semid, 0, GETVAL, 0));
    while (1)
    {
        sem_v(semid);
        printf("In main, sleep several seconds.\n");
        sleep(2);
    }
    return 0;
}