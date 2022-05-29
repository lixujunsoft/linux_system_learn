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

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} un;

void p(int semid , int num)
{
    struct sembuf buf = {num, -1, 0};
    if (semop(semid, &buf, 1) == -1) {
        perror("p r");
        exit(-1);
    }
}

void v(int semid, int num)
{
    struct sembuf buf = {num, 1, 0};
    if (semop(semid, &buf, 1) == -1) {
        perror("v w");
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
    int shmid, semid, s[] = {1, 0};  //声明一个共享内存id和一个信号灯集id
    char *shmaddr = NULL;  //用于存放共享内存的起始地址
    struct shmid_ds buf;

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
        p(semid, R);   //读之前申请读资源
        printf("lixjun\n");
        printf("%s", shmaddr);  //将共享内存中内容输出到屏幕上
        if (strncmp(shmaddr, "quit", 4) == 0)  //看是否退出
        {
		/*
		//退出之前先发一个读资源，完成唤醒另一个读者的作用，当写进程退出后，保证所有的读者全部退出，
		而最后退出的读者删除共享内存和信号灯集
		*/
           
		    v(semid, R);  
            break;
        }
        v(semid, W);  //发送写信号
    }

    if (shmdt(shmaddr) == -1) { // 解除该进程的共享内存映射
        perror("shmdt");
        exit(-1);
    }

    if (shmctl(shmid, IPC_STAT, &buf) == -1) { //读取共享内存的信息
        perror("shmctl");
        exit(-1);
    }
    
    if (buf.shm_nattch == 0) { //判断该共享内存的映射数是否为0，说明这已经是最后一个读者了。如果是的话，删除共享内存和信号灯集
        if (shmctl(shmid, IPC_RMID, NULL) == -1) { //删除共享内存
            perror("smmctl");
            exit(-1);
        }
        if (semctl(semid, 0, IPC_RMID, NULL) == -1) { //删除信号灯集
            perror("sem rm");
            exit(-1);
        }
    }

	exit(0);
}