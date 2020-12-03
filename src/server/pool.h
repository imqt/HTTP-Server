#ifndef DC_POOL_H
#define DC_POOL_H

#include "../dc_lib/sys/socket.h"
#include "../dc_lib/unistd.h"
#include "../http/response.h"
#include "../http/request.h"

#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define BACKLOG 5
#define DEALERS 11

void threadz(int sfd, int n);

void processez(int sfd, int n);

void *dealer(void *vargp);

#endif //DC_POOL_H
