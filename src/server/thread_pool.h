//
// Created by wuviv on 2020-11-15.
//

#ifndef DC_THREAD_POOL_H
#define DC_THREAD_POOL_H

#include <pthread.h>

//https://github.com/pminkov/threadpool/blob/1dd4f37bea012e9c46975d625b480a0c84d53b06/threadpool.h
typedef struct thread_pool_s thread_pool;

//Create a thread pool and returns a pointer to it.
thread_pool *pool_init(int max_threads);

//Return maximum number of threads
int pool_get_max_threads(thread_pool *);

//Insert task into thread pool. The pool will call work_routine and pass arg
//as an argument to it. This is a similar interface to pthread_create.
void pool_add_task(thread_pool *, void*(*work_routine)(void*), void* arg);

//Blocks until the thread pool is done executing its tasks.
void pool_wait(thread_pool *);

//Cleans up the thread pool, frees memory. Waits until work is done.
void pool_destroy(thread_pool *);

#endif //DC_THREAD_POOL_H
