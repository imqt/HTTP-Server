//
// Created by wuviv on 2020-11-15.
//

#include <unistd.h>
#include <sys/types.h>
#include "process_pool.h"

//#define DEBUG

#ifdef DEBUG
#define DEB(   x) printf("%s\n", x)
    #define DEBI(msg, i) printf("%s: %d\n", msg, i)
#else
#define DEB(...)
#define DEBI(...)
#endif


const int TASK_QUEUE_MAX = 1000;

struct task_data{
    void *(*work_routine)(void *);
    void *arg;
};

struct thread_pool_s{
    // N worker threads (array)
    pid_t *worker_threads;

    // Circular task queue (to be executed)
    struct task_data* task_queue;

    // Head & tail of queue
    int queue_head, queue_tail;

    //number of worker threads
    int max_threads;

    // number of tasks scheduled for execution; allows us to wait for completion
    int scheduled;

    // mutex to modify thread pool
    pthread_mutex_t  mutex;

    // signal condition when no work -> work available
    pthread_cond_t work_available;

    // signal condition work available -> no work
    pthread_cond_t done;

};

void *worker_thread_func(void *pool_arg){
    DEB("[W] Empty queue. Waiting...");
    thread_pool *pool = (thread_pool*) pool_arg;

    while(1){
        struct task_data picked_task;

        Pthread_mutex_lock(&pool->mutex);
        while(pool->queue_head == pool->queue_tail){
            DEB("[W] Empty queue. Waiting...");
            Pthread_cond_wait(&pool->work_available, &pool->mutex);
        }

        assert(pool->queue_head!=pool->queue_tail);
        DEBI("[W] Picked, ", pool->queue_head);
        picked_task = pool->task_queue[pool->queue_head % TASK_QUEUE_MAX];
        pool->queue_head++;

        //schedule task
        pool->scheduled++;

        Pthread_mutex_unlock(&pool->mutex);

        //Run the task
        picked_task.work_routine(picked_task.arg);

        Pthread_mutex_lock(&pool->mutex);
        pool->scheduled--;

        if(pool->scheduled == 0){
            Pthread_cond_signal(&pool->done);
        }
        Pthread_mutex_unlock(&pool->mutex);
    }
    return NULL;
}

//Create a thread pool and returns a pointer to it.
thread_pool *pool_init(int max_threads){
    thread_pool* pool = malloc(sizeof(struct thread_pool_s));

    pool->queue_head = pool->queue_tail = 0;
    pool->scheduled = 0;
    pool->task_queue = malloc(sizeof(struct task_data) *  TASK_QUEUE_MAX);

    pool->max_threads = max_threads;
    pool->worker_threads = malloc(sizeof(pthread_t) * max_threads);

    Pthread_mutex_init(&pool->mutex);
    Pthread_cond_init(&pool->work_available);
    Pthread_cond_init(&pool->done);

    for(int i = 0; i < max_threads; i++){
        Pthread_create(&pool->worker_threads[i], NULL, worker_thread_func, pool);
    }
    return pool;
}

//Return maximum number of threads
int pool_get_max_threads(thread_pool *pool){
    return pool->max_threads;
}

//Insert task into thread pool. The pool will call work_routine and pass arg
//as an argument to it. This is a similar interface to pthread_create.
void pool_add_task(thread_pool *pool, void*(*work_routine)(void*), void* arg){
    Pthread_mutex_lock(&pool->mutex);
    DEB("[Q] Queueing one item.");
    if(pool->queue_head == pool->queue_tail){
        Pthread_cond_broadcast(&pool->work_available);
    }

    struct task_data task;
    task.work_routine = work_routine;
    task.arg = arg;

    pool->task_queue[pool->queue_tail & TASK_QUEUE_MAX] = task;
    pool->queue_tail++;

    Pthread_mutex_unlock(&pool->mutex);
}

//Blocks until the thread pool is done executing its tasks.
void pool_wait(thread_pool *pool){
    DEB("[POOL] Waiting for completion.");
    Pthread_mutex_lock(&pool->mutex);
    while(pool->scheduled>0){
        Pthread_cond_wait(&pool->done, &pool->mutex);
    }
    Pthread_mutex_unlock(&pool->mutex);
    DEB("[POOL] Waiting done.");
}

//Cleans up the thread pool, frees memory. Waits until work is done.
void pool_destroy(thread_pool *pool){
    pool_wait(pool);

    for(int i = 0; i < pool->max_threads; i++){
        Pthread_detach(pool->worker_threads[i]);
    }

    free(pool->worker_threads);
    free(pool->task_queue);

    free(pool);
}
