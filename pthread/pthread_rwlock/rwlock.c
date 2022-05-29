/*1. 读写锁和互斥锁的区别
   读写锁和互斥锁都可以解决多线程在访问共享资源竞争的问题，不同的是读写锁允许
   多个线程同时读某个共享变量，在同一时刻只允许一个线程对变量进行写操作，因此
   读写锁的效率要高于互斥锁。

2. 读写锁是几把锁
   读写锁其实是一把锁，但是这把锁拥有读和写两种属性

3. 读写锁的三种状态
   写加锁，此时其它任何试图获取读锁或写锁的线程都将被阻塞，这种属性称为写独占；
   读加锁，此时其它任何试图获取读锁的线程都可以加锁成功，这种属性被称为读共享；
   读加锁，此时如果既有线程试图获取读锁，又有线程试图获取写锁，那么系统会优先让线程获得写锁并阻塞获取读锁的线程，也可以简单理解为写锁的优先级高于读锁；

4. 应用场景
   读写锁非常适用于数据的读次数远高于写次数的场景，因为这样可以大大提高线程的执行效率；
   举个简单的例子，比如有100个线程同时需要读取变量x的值，它们每个线程执行完锁需要的时
   间是1ms.如果使用读写锁，这些线程的状态则为并行，即100个线程执行完只需要1ms。而如
   果使用互斥锁，这些线程的状态则为串行，这些线程执行完需要的时间大约为100ms。
   使用读写锁可以保证线程在写的时候不会产生读的动作，在读的时候不会产生写的动作,从而保证了数据安全

5. 适用范围
   读写锁只适用于对变量、内存以及队列、链表等的操作，不适用于硬件接口，比如使用spi读写flash数据，对于硬件接口相关的访问只能使用互斥锁或者自旋锁。

6. 读写锁常用函数
   6.1 pthread_rwlock_init // 初始化读写锁
    函数原型： int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    函数功能：初始化读写锁
    函数参数：rwlock 读写锁句柄
            attr 读写锁参数，如果这个值为NULL，则使用系统默认的读写锁参数
    返回值：成功返回0，失败返回对应的错误码

   6.2 pthread_rwlock_rdlock和pthread_rwlock_tryrdlock // 获取读锁
   函数原型：int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
           int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);

   函数功能：这两个函数都可以获取读锁，不同的是pthread_rwlock_rdlock以阻塞的方式获取读锁，
           pthread_rwlock_tryrdlock以非阻塞的方式获取读锁
   函数参数：rwlock 读写锁句柄
   返回值：成功返回0，失败返回对应的错误码

   6.3 pthread_rwlock_wrlock和pthread_rwlock_trywrlock // 获取写锁
   函数原型： int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
            int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
   函数功能：这两个函数都可以获取写锁，不同的是pthread_rwlock_wrlock以阻塞的方式获取读锁，
           pthread_rwlock_trywrlock以非阻塞的方式获取读锁
   函数参数：rwlock 读写锁句柄
   返回值：成功返回0，失败返回对应的错误码

   6.4 pthread_rwlock_timedrdlock和pthread_rwlock_timedwrlock // 以超时的方式获取读锁和写锁
   函数原型： int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock, const struct timespec *restrict abs_timeout);
            int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock, const struct timespec *restrict abs_timeout);
   函数功能：以超时的方式获取读锁和写锁

   6.5 pthread_rwlock_unlock 解锁读写锁
   函数原型：int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
   函数功能：解锁读写锁
   */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

pthread_attr_t g_attr;
pthread_t tid1[11], tid2[11];
pthread_rwlock_t g_wr_lock;
unsigned int g_count = 0;

void set_thread_pri(pthread_t id, int prio)
{
   struct sched_param pri;
   pri.sched_priority = prio;
   if (0 != pthread_setschedparam(id, SCHED_FIFO, &pri)) {
      printf("thread shed policy set fail\n");
   }
}

void *pthread1(void *param)
{
   int id = (int)param;
   sleep(1);
   pthread_detach(pthread_self());
   while (g_count < 100) {
      pthread_rwlock_wrlock(&g_wr_lock);
      g_count++;
      printf("%d thread get write_lock, count = %d\n", id, g_count);
      pthread_rwlock_unlock(&g_wr_lock);
      sleep(1);
   }
}

void *pthread2(void *param)
{
   int id = (int)param;
   // set_thread_pri(pthread_self(), 10);
   sleep(1);
   pthread_detach(pthread_self());
   while (g_count < 100) {
      pthread_rwlock_rdlock(&g_wr_lock);
      printf("%d thread get read_lock, count = %d\n", id, g_count);
      pthread_rwlock_unlock(&g_wr_lock);
      sleep(1);
   }
}

int main()
{
   int ret = -1;
   int i = 0;
   unsigned int stacksize = 0;
   struct sched_param pri;
   int detachstate;
   void *stachaddr;

   // 设置进程调度方式和优先级，所有线程的默认调度方式和优先级同进程
   /*
   pri.sched_priority = 11;
   if (0 != sched_setscheduler(getpid(), SCHED_FIFO, &pri)) {
      printf("process shed policy set fail\n");
   }*/

   // 初始化读写锁
   ret = pthread_rwlock_init(&g_wr_lock, NULL);
   if (0 != ret) {
      printf("wr lock init error\n");
      return -1;
   }

   for (i = 0; i < 10; i++) {
      pthread_create(&tid1[i], NULL, pthread1, (void*)i);  // 线程attr为NULL表示使用系统默认属性
   }

   for (i = 0; i < 10; i++) {
      pthread_create(&tid2[i], NULL, pthread2, (void*)i);
   }

   while (1) {
      sleep(100);
   }
   return 0;
}

