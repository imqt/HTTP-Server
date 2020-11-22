#include <errno.h>
#include <stddef.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "shared_mutex.h"

shared_mutex_t shared_mutex_init(char* name){
    shared_mutex_t mutex = {NULL, 0 , NULL, 0};
    errno = 0;

    mutex.shm_fd = shm_open(name, O_RDWR, 0660);
    if(errno == ENOENT){
        mutex.shm_fd = shm_open(name, O_RDWR|O_CREAT, 0660);
        mutex.created = 1;
    }
    if(mutex.shm_fd == -1){
        perror("shm_open");
        return mutex;
    }

    if(ftruncate(mutex.shm_fd, sizeof(pthread_mutex_t))!=0){
        perror("ftruncate");
        return mutex;
    }

    void *addr = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_)


}

int shared_mutex_close(shared_mutex_t mutex){

}

int shared_mutex_destroy(shared_mutex_t mutex);