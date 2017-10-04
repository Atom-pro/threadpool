#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>
#include <stdio.h>

typedef struct condtion{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}condition_t;

void condition_init(condition_t *cond);
void condition_lock(condition_t *cond);
void condition_unlock(condition_t *cond);
void condition_wait(condition_t *cond);
int condition_timedwait(condition_t *cond, const struct timespec *abstime);
void condition_signal(condition_t *cond);
void condition_broadcast(condition_t *cond);
void condition_destroy(condition_t *cond);

#endif //__CONDITION_H__

