#include "condition.h"

void condition_init(condition_t *cond)
{
	pthread_mutex_init(&cond->mutex, NULL);
	pthread_cond_init(&cond->cond, NULL);
}

void condition_lock(condition_t *cond)
{
	pthread_mutex_lock(&cond->mutex);
}

void condition_unlock(condition_t *cond)
{
	pthread_mutex_unlock(&cond->mutex);
}

void condition_wait(condition_t *cond)
{
	pthread_cond_wait(&cond->cond, &cond->mutex);
}

int condition_timedwait(condition_t *cond, const struct timespec *abstime)
{
	return pthread_cond_timedwait(&cond->cond, &cond->mutex, abstime);
}

void condition_signal(condition_t *cond)
{
	pthread_cond_signal(&cond->cond);
}

void condition_broadcast(condition_t *cond)
{
	pthread_cond_broadcast(&cond->cond);
}

void condition_destroy(condition_t *cond)
{
	pthread_mutex_destroy(&cond->mutex);
	pthread_cond_destroy(&cond->cond);
}

