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

union  semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main(int argc, char *argv[])
{
    const char *name = "./a.c";
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
    }

    int ret = semctl(semid, 0, IPC_RMID);
    // shell cmd: ipcrm -s semid 
    if (-1 == ret) {
        printf("semctl error: %s\n", strerror(errno));
        return -1;
    }
    printf("semid %d delete succ\n", semid);
    return 0;
}
