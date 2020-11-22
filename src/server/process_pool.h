//
// Created by wuviv on 2020-11-15.
//

#ifndef DC_PROCESS_POOL_H
#define DC_PROCESS_POOL_H

//https://github.com/pminkov/threadpool/blob/1dd4f37bea012e9c46975d625b480a0c84d53b06/threadpool.h
typedef struct process_pool_s process_pool;

//Create a thread pool and returns a pointer to it.
process_pool *pool_init(int max_threads);

//Return maximum number of threads
int pool_get_max_threads(process_pool *);

//Insert task into thread pool. The pool will call work_routine and pass arg
//as an argument to it. This is a similar interface to pthread_create.
void pool_add_task(process_pool *, void*(*work_routine)(void*), void* arg);

//Blocks until the thread pool is done executing its tasks.
//void pool_wait(process_pool *);

//Cleans up the thread pool, frees memory. Waits until work is done.
void pool_destroy(process_pool *);
#endif //DC_PROCESS_POOL_H
