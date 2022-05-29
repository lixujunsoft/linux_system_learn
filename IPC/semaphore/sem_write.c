#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#define N 1024

#define R 0
#define W 1

union semun    //声明一个共用体，并定义一个共用体变量，该共用体的具体信息可以 man semctl
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} un;


void p(int semid , int num)
{
    struct sembuf buf = {num, -1, 0};
    if (semop(semid, &buf, 1) == -1) {
        perror("p w");
        exit(-1);
    }
}
void v(int semid, int num)
{
    struct sembuf buf = {num, 1, 0};
    if (semop(semid, &buf, 1) == -1) {
        perror("v r");
        exit(-1);
    }
}

int initSem(int semid, int s[], int n)
{
    int i;
    union semun myun;
    for (i = 0; i < n; i++) {
        myun.val = s[i];
        if (semctl(semid, i, SETVAL, myun) == -1) {
            return -1;
        }
    }
}

int main()
{
	key_t key;  //声明一个密钥
    int shmid, semid, s[] = {0, 1};   //声明一个共享内存id和一个信号灯集id
    char *shmaddr = NULL;  //用于存放共享内存的起始地址

	if ((key = ftok(".", 'a')) == -1) {
		perror("ftok");
		exit(-1);
	}

    semid = semget(key, 2, 0666 | IPC_CREAT | IPC_EXCL);
    if (-1 == semid) {
        if (errno != EEXIST) {
            perror("semget");
            exit(-1);
        }
        semid = semget(key, 0, 0666);
    }
    
    printf("semget succ\n");
    if(initSem(semid, s, 2) == -1) {
        perror("semctl error");
        exit(-1);
    }

    if ((shmid = shmget(key, N, 0666 | IPC_CREAT)) == -1) { //得到共享内存的id
        perror("shmget");
        exit(-1);
    }

    if ((shmaddr = (char *)shmat(shmid, NULL, 0)) < (char *)0) { //得到共享内存映射后的起始地址
        perror("shmat");
        exit(-1);
    }

    while (1) {
        p(semid, W);  //写之前先申请写资源
        fgets(shmaddr, N, stdin);  //将输入的内容写到共享内存
        v(semid, R);  //发送读信号
        if (strncmp(shmaddr, "quit", 4) == 0)  //看是否退出
            break;
    }

    if (shmdt(shmaddr) == -1) { //退出后，取消该进程对共享内存的映射，它的删除有另一方完成
        perror("shmdt");
        exit(-1);
    }

	exit(0);
}

