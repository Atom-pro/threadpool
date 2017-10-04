/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Oct 2017 08:33:58 PM PDT
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"
#include"condition.h"

void *mytask(void *arg)
{
    int i = *(int*)arg;
    free(arg);
    printf("%X thread runing on task %d\n", pthread_self(), i);
    sleep(1);
}

int main()
{
    threadpool_t pool;
    threadpool_init(&pool, 3);
    

    int i;
    for(i=0; i<10; ++i){
        int* p = malloc(sizeof(int));
        *p = i;
        threadpool_add_task(&pool, mytask, (void*)p);
    }
    //sleep(15);
    threadpool_destroy(&pool);
}
