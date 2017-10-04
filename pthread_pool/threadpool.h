#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "condition.h"

// 任务节点
typedef struct task {
	void *(*run)(void *arg);	// 回调函数
	void *arg;					// 回调函数的参数
	struct task *next;
}task_t;

// 线程池的结构体
typedef struct threadpool {
	condition_t ready;  // 同步和互斥的条件
	task_t *first;      // 任务队列的队头指针
	task_t *last;		// 任务队列的队尾指针
	int counter;		// 当前的线程数
	int idle;			// 空闲线程数
	int max_threads;	// 线程池允许的最大线程数
	int quit;			// 线程的退出标记，1表示退出
}threadpool_t;

// 初始化线程池
// @pool - 线程池对象
// @threads - 线程池的最大数量
void threadpool_init(threadpool_t *pool, int threads);

// 向线程池中添加任务
// @run - 任务回调函数
void threadpool_add_task(threadpool_t *pool, void *(*run)(void*), void *arg);

// 销毁线程池
void threadpool_destroy(threadpool_t *pool);

#endif //__THREADPOOL_H__

