#include <stdlib.h>
#include "config.h"
Config create_config(char * path_404, char * root, size_t port, int concurr_opt, int ipc_opt){
    Config this = (Config) calloc(1, sizeof(struct Config_S));

    this->path_404 = path_404;
    this->root = root;
    this->port = port;
    this->concurr_opt = concurr_opt;
    this->ipc_opt = ipc_opt;
}

void delete_config(Config this){
    free(this);
}