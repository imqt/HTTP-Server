//
// Created by wuviv on 2020-11-15.
//

#include <unistd.h>
#include <sys/types.h>
#include "process_pool.h"

const int TASK_QUEUE_MAX = 1000;

struct process_pool_s{
    fd_set active_fd_set, read_fd_set;
    int inet_fd;
};

typedef struct process_pool_s* process_pool;
