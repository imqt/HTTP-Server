#ifndef DC_CONFIG_H
#define DC_CONFIG_H

#include <stddef.h>

#define CONCURR_OPT_THREAD 0
#define CONCURR_OPT_PROCESS 1

struct Config_S{
    char * server_name;
    size_t port;
    char * root;
    int connections;
    char * path_home;
    char * path_404;
    int concurr_opt; //0: threads 1: processes
    int backlog;
    int exit;
};
typedef struct Config_S* Config;

Config config_create();
void config_delete(Config this);
void config_set_default(Config c);
void config_set_file(Config c);
void config_print(const Config c);

void *config_handler(void *vargp);
#endif //DC_CONFIG_H
