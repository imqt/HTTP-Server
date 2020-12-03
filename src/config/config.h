//
// Created by wuviv on 2020-11-04.
//

#ifndef DC_CONFIG_H
#define DC_CONFIG_H

#include <stddef.h>

#define CONCURR_OPT_THREAD 0
#define CONCURR_OPT_PROCESS 1

#define IPC_OPT_SOCKET 0
#define IPC_OPT_PIPE 1
#define IPC_OPT_SEMAPHORE 2
#define IPC_OPT_SHAREMEM 3

struct Config_S{
    char * server_name;
    size_t port;
    char * root;
    int connections;
    char * path_home;
    char * path_404;
    int concurr_opt; //0: threads 1: processes
};
typedef struct Config_S* Config;

Config get_config(char * filepath);
void parse_option()
void delete_config(Config this);
#endif //DC_CONFIG_H
