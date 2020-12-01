//
// Created by wuviv on 2020-11-15.
//

#ifndef DC_PROCESS_POOL_H
#define DC_PROCESS_POOL_H

//https://github.com/pminkov/threadpool/blob/1dd4f37bea012e9c46975d625b480a0c84d53b06/threadpool.h
typedef struct process_pool_s* process_pool;

//Create a thread pool and returns a pointer to it.
process_pool pool_init(int max_threads, void*(*process_task)(void*));

//Return maximum number of threads
void pool_start(process_pool *);

//Blocks until the thread pool is done executing its tasks.
void pool_wait(process_pool *);

//Cleans up the thread pool, frees memory. Waits until work is done.
void pool_destroy(process_pool *);
#endif //DC_PROCESS_POOL_H
