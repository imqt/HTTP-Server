
#ifndef DC_SHARED_MUTEX_H
#define DC_SHARED_MUTEX_H

//https://gist.github.com/yamnikov-oleg/abf61cf96b4867cbf72d
typedef struct shared_mutex_t_s{
    pthread_mutex_t *ptr;
    int shm_fd;
    char* name;
    int created;
}shared_mutex_t;

shared_mutex_t shared_mutex_init(char* name);

int shared_mutex_close(shared_mutex_t mutex);

int shared_mutex_destroy(shared_mutex_t mutex);

#endif //DC_SHARED_MUTEX_H
