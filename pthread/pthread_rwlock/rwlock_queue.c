#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

typedef void (*pFunc)(int number);

struct job {
    struct job *j_next;
    struct job *j_prev;
    pthread_t j_id;
    pFunc func;
    int number;
};

struct queue {
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

struct queue *g_qp;
pthread_t master, work1, work2;

void showNumber(int number)
{
    sleep(1);
    printf("%d\n", number);
}

int queue_init(struct queue *qp)
{
    int err;
    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0) {
        return err;
    }
    return 0;
}

void job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL) {
        qp->q_head->j_prev = jp;
    } else {
        qp->q_tail = jp;
    }
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->q_tail != NULL) {
        qp->q_tail->j_next = jp;
    } else {
        qp->q_head = jp;
    }
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp) {
            qp->q_tail = NULL;
        } else {
            jp->j_next->j_prev = jp->j_prev;
        }
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

struct job * job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0) {
        return NULL;
    }

    for (jp = qp->q_head; jp != NULL; jp = jp->j_next) {
        if (pthread_equal(jp->j_id, id)) {
            break;
        }
    }

    pthread_rwlock_unlock(&qp->q_lock);
    return jp;
}

void *master_handle(void *input)
{
    int flag = 0;
    struct job *myjob;

    while (1)
    {
        myjob = (struct job*)malloc(sizeof(struct job));
        myjob->func = showNumber;
        myjob->j_next = NULL;
        myjob->j_prev = NULL;
        myjob->number = flag;

        if (flag % 2 == 0) {
            myjob->j_id = work1;
        } else {
            myjob->j_id = work2;
        }
        job_append(g_qp, myjob);
        flag++;
        sleep(1);
    }
    
}

void *work_handle(void *input)
{
    while (1)
    {
        printf("work_handle:%d\n", *((pthread_t *)input));
        struct job* currentJob = job_find(g_qp, *((pthread_t *)input));
        if  (NULL == currentJob) {
            sleep(1);
            continue;
        }
        currentJob->func(currentJob->number);
        job_remove(g_qp, currentJob);
    }
}

int main()
{
    g_qp = malloc(sizeof(struct queue));
    queue_init(g_qp);
    pthread_create(&master, NULL, master_handle, NULL);
    pthread_create(&work1, NULL, work_handle, &work1);
    pthread_create(&work2, NULL, work_handle, &work2);
    pthread_join(master, NULL);
    pthread_join(work1, NULL);
    pthread_join(work2, NULL);

    return 0;
}