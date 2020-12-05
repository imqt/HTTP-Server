#ifndef DC_POOL_H
#define DC_POOL_H

#include "../dc_lib/sys/socket.h"
#include "../dc_lib/unistd.h"
#include "../http/response.h"
#include "../http/request.h"
#include "../config/config.h"

#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

void threadz(int sfd, Config config, sem_t* config_mutex);

void processez(int sfd, Config config, sem_t* config_mutex);

void *dealer(void *vargp);

#endif //DC_POOL_H
