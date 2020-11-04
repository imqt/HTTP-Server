//
// Created by wuviv on 2020-11-04.
//

#ifndef DC_CONFIG_H
#define DC_CONFIG_H

#include <stddef.h>

struct Config_S{
    char * path_404;
    char * root;
    size_t port;
    int concurr_opt; //0: threads 1: processes
    int ipc_opt;     //0: sockets 1: pipes 2: semaphores 3: shared memory
};
typedef struct Config_S* Config;

Config get_config();
Config set_config(char * path_404, char * root, size_t port, int concurr_opt, int ipc_opt);
void delete_config(Config this);
#endif //DC_CONFIG_H
