/*
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

    信号灯与其他进程间通信方式不大相同，它主要提供对进程间共享资源访问控制机制。相当于内存中的标志，
进程可以根据它判定是否能够访问某些共享资源，同时，进程也可以修改该标志。除了用于访问控制外，还可
用于进程同步。信号灯有以下两种类型：

a.二值信号灯：最简单的信号灯形式，信号灯的值只能取0或1，类似于互斥锁。
    注：二值信号灯能够实现互斥锁的功能，但两者的关注内容不同。信号灯强调共享资源，只要共享资源可用，
    其他进程同样可以修改信号灯的值；互斥锁更强调进程，占用资源的进程使用完资源后，必须由进程本身来解锁。
b.计数信号灯：信号灯的值可以取任意非负值（当然受内核本身的约束）

1.创建或打开函数
原型：int semget(key_t key, int nsems, int semflag)
参数：key 和信号灯集合关联的键值
     nsems 信号灯集合包含的信号量数目
     semflag 信号灯集合的访问权限
返回值：成功返回信号灯集合ID，出错返回-1

2. 关闭
原型：int semctl(int semid, int semnum, int cmd, ...union semun arg)
     注意最后一个参数不是地址，可以有，可以没有
参数：semid 信号灯集合ID
     semnum 要修改的信号灯的编号；删除操作时，这个值可以设置为任意值
     cmd GETVAL    获取信号灯集中信号灯的值，返回值是获取到的信号灯值，例如： value = semctl(semid, 0, GETVAL)；
         SETVAL    将Val值赋给信号灯集合中semnum位置的信号灯
         IPC_RMID  从系统中删除信号灯集

     union semun arg: union semun {
                                int                  val;    // Value for SETVAL
                                struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET
                                unsigned short  *array;  // Array for GETALL, SETALL
                                struct seminfo  *__buf;  // Buffer for IPC_INFO(Linux-specific)
3.P/V操作函数
P/V操作函数semop
原型：int semop(int semid, struct sembuf *opsptr, size_t nops)
参数：semid 信号灯ID
     opsptr struct sembuf {
         short sem_num;    // 要操作的信号量在信号灯集中的编号
         short sem_op;     // 0: 等待； 1：资源释放，V操作； -1：分配资源，P操作
                           // 信号灯的当前值记录相应资源目前可用数目；sem_op > 0对应相应进程要释放sem_op数
                              目的共享资源；sem_op=0可以用于对共享资源是否已用完的测试；sem_op<0相当于进程要申请-sem_op个共享资源
         short sem_flg;    // 0: IPC_NOWAIT, SEM_UNDO
                           // 说明函数semop的行为。通常被设置为SEM_UNDO。它将使得操作系统跟着当前进程对这
                              个信号量的修改情况，如果这个进程在没有释放该信号量的情况下终止，操作系统将自动
                              放该进程持有的信号量。用一个通俗的说法：IPC_UNDO标志保证进程终止后，它对信
                              量的修改都撤销，好像它从来没有操作过信号量一样
     }
     nops：要操作的信号灯的个数
返回值：成功，消息队列ID，出错-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define N 64
#define READ 0
#define WRITE 1

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void init_sem(int semid, int s[], int n)
{
    int i; 
    union semun myun;
    for (i = 0; i < n; i++) {
        myun.val = s[i];
        semctl(semid, i, SETVAL, myun);
    }
}

void pv(int semid, int num, int op)
{
    struct sembuf buf;

    buf.sem_num = num;
    buf.sem_op = op;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

int main()
{
    int shmid, semid, s[] = {0, 1};
    pid_t pid;
    key_t key;
    char *shmaddr;

    key = ftok("./a.c", 's');
    if (key == -1) {
        perror("ftok");
        exit(-1);
    }

    shmid = shmget(key, N, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmid");
        exit(-1);
    }

    semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        goto __ERROR1;
    }
    init_sem(semid, s, 2);

    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == NULL) {
        perror("shmaddr");
        goto __ERROR2;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        goto __ERROR2;
    } else if (pid == 0) {
        char *p, *q;
        while (1) {
            pv(semid, READ, -1);
            p = q = shmaddr;
            while (*q) {
                if (*q != ' ') {
                    *p++ = *q;
                }
                q++;
            }
            *p = '\0';
            printf("child: %s", shmaddr);
            pv(semid, WRITE, 1);
        }
    } else {
        while (1) {
            union semun myunGet;
            printf("myunGet.val = %d\n", semctl(semid, WRITE, GETVAL));
            pv(semid, WRITE, -1);
            printf("input >\n");
            fgets(shmaddr, N, stdin);
            if (strcmp(shmaddr, "quit\n") == 0) {
                break;
            }
            pv(semid, READ, 1);
        }
    }

__ERROR2:
    semctl(semid, 0, IPC_RMID);
__ERROR1:
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}