#include <stdlib.h>
#include "config.h"

Config get_config(char * filepath){
    Config this = (Config) calloc(1, sizeof(struct Config_S));

    this->server_name
    this->path_404 = "/";
    this->root = "/";
    this->port = 80;
    this->concurr_opt = CONCURR_OPT_PROCESS;
    return this;
}

Config set_config(char * path_404, char * root, size_t port, int concurr_opt, int ipc_opt){
    Config this = (Config) calloc(1, sizeof(struct Config_S));

    this->path_404 = path_404;
    this->root = root;
    this->port = port;
    this->concurr_opt = concurr_opt;
    return this;
}

void delete_config(Config this){
    free(this);
}