#include "threadpool.h"
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

// 线程池的结构体
//typedef struct threadpool {
//	condition_t ready;  // 同步和互斥的条件
//	task_t *first;      // 任务队列的队头指针
//	task_t *last;		// 任务队列的队尾指针
//	int counter;		// 当前的线程数
//	int idle;			// 空闲线程数
//	int max_threads;	// 线程池允许的最大线程数
//	int quit;			// 线程的退出标记，1表示退出
//}threadpool_t;

void *rout(void* arg)
{
    threadpool_t* pool = (threadpool_t*)arg;
    //printf("%X thread is strating\n", pthread_self());
    int timeout = 0;
    while(1){
        condition_lock(&pool->ready);
        pool->idle++;//是一个空线程

        //当任务队列为空, 并且没有得到退出标志, 就等待
        while(pool->first == NULL && pool->quit == 0){
            printf("%X thread is waiting %d %d\n", pool->counter, pool->idle);
            struct timespec ts;            
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += 2;
            int t = condition_timedwait(&pool->ready, &ts);
            if(t == ETIMEDOUT){
                printf("%X thread time out %d %d\n", pthread_self(), pool->counter, pool->idle); 
                timeout = 1;
                break;
            }
        }
        
        pool->idle--;//等待结束，处于工作状态
        printf("%X thread is strating  %d  %d\n", pthread_self(), pool->counter, pthread_self());

        //任务队列有任务
        if(pool->first != NULL){
            task_t *p = pool->first;//取出队头节点
            pool->first = p->next;

            //防止run函数执行的时间长
            condition_unlock(&pool->ready);
            p->run(p->arg);
            condition_lock(&pool->ready);

            free(p);
        }

        //退出
        if(pool->quit == 1 && pool->first == NULL){
            pool->counter--;
            if(pool->counter == 0){
                condition_signal(&pool->ready);
            }
            condition_unlock(&pool->ready);
            break;
        }
        
        //超时处理
        if( timeout == 1 && pool->first == NULL ){
            pool->counter--;
            condition_unlock(&pool->ready);
            break;
        }
        condition_unlock(&pool->ready);
    }
}


void threadpool_init(threadpool_t *pool, int threads)//
{
    condition_init(&pool->ready);
    pool->first       =   NULL;
    pool->last        =   NULL;
    pool->counter     =   0;
    pool->idle        =   0;
    pool->max_threads =   threads;
    pool->quit        =   0;
}

// 向线程池中添加任务
// @run - 任务回调函数
void threadpool_add_task(threadpool_t *pool, void *(*run)(void*), void *arg)
{
    task_t *p = (task_t*)malloc(sizeof(task_t));
    memset(p, 0, sizeof(task_t));
    p->run  = run;  //xingcan
    p->arg  = arg;
    p->next = NULL;

    condition_lock(&pool->ready);
    if(pool->first == NULL){
        pool->first = p;
    }else{
        pool->last->next = p;
    }
    pool->last = p;

    if(pool->idle > 0){
        condition_signal(&pool->ready);
    }else if(pool->counter < pool->max_threads) {
        printf("++++++++++++++++++++++pool->counter=%d, pool->max_threads=%d\n", pool->counter, pool->max_threads);
        pool->counter++;
        pthread_t tid;
        pthread_create(&tid, NULL, rout, (void*)pool);
    }

    condition_unlock(&pool->ready);
}

// 销毁线程池
void threadpool_destroy(threadpool_t *pool)
{
    if(pool->quit == 1){
        return;
    }
    condition_lock(&pool->ready); 

    pool->quit = 1;
    if(pool->idle > 0){
        condition_broadcast(&pool->ready);
    }

    while(pool->counter > 0){
        condition_wait(&pool->ready);
    }
    condition_unlock(&pool->ready); 
    condition_destroy(&pool->ready);
}

