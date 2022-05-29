#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>
   
pthread_rwlock_t rwlock;//读写锁的创建
 
void * read_fun1(void * arg)   //读子线程的函数
{
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);   //对读操作枷锁
        printf("read1 start \n");
        // sleep(1);
        usleep(1);
        printf("read2 end \n");
        pthread_rwlock_unlock(&rwlock);   //读完进行解锁
        // sleep(1);
        usleep(1);
    }
} 
void * read_fun2(void * arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);   //对读操作枷锁
        printf("read2 start \n");
        // sleep(1);
        usleep(1);
        printf("read2 end \n");
        pthread_rwlock_unlock(&rwlock);   //读完进行解锁
        // sleep(1);
        usleep(1);
    }
}
  
void * write_fun(void * arg)
{
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);   //对写操作枷锁
        printf("write start \n");
        // sleep(1);
        usleep(1);
        printf("write end  \n");
        pthread_rwlock_unlock(&rwlock);   //写完进行解锁
        // sleep(1);
        usleep(1);
    }
}
 
int main()
{
    pthread_rwlock_init(&rwlock,NULL);
    pthread_t idr1,idr2; //创建两个读的线程
    pthread_t idw;  //创建一个写的线程
 
    pthread_create(&idr1,NULL,read_fun1,NULL);
    pthread_create(&idr2,NULL,read_fun2,NULL);
    pthread_create(&idw,NULL,write_fun,NULL);
 
    pthread_join(idr1,NULL);
    pthread_join(idr2,NULL);
    pthread_join(idw,NULL);
 
    pthread_rwlock_destroy(&rwlock);
    exit(0);
}