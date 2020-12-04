#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "../dc_lib/unistd.h"

#define DEFAULT_SERVER_NAME "Team-7"
#define DEFAULT_ROOT "../../rsc/"
#define DEFAULT_404 "../../rsc/404.html"
#define DEFAULT_INDEX "../../rsc/index.html"
#define DEFAULT_PORT 49512
#define DEFAULT_CONNECTIONS 10
#define DEFAULT_BACKLOG 5

#define BUFF_SIZE 100000

Config config_create(){
    Config this = (Config) calloc(1, sizeof(struct Config_S));
    config_set_default(this);
    config_set_file(this);
    return this;
}

void config_delete(Config this){
    free(this);
}

void config_set_default(Config c){
    c->server_name = DEFAULT_SERVER_NAME;
    c->port = DEFAULT_PORT;
    c->root = DEFAULT_ROOT;
    c->connections = DEFAULT_CONNECTIONS;
    c->path_home = DEFAULT_INDEX;
    c->path_404 = DEFAULT_404;
    c->concurr_opt = CONCURR_OPT_THREAD;
    c->backlog = DEFAULT_BACKLOG;
}

void config_set_file(Config c){
    int config_in;
    if((config_in=open("../edit-config.txt", O_RDONLY))==-1){
        perror("Open");
    }

    char buf[BUFF_SIZE];
    dc_read(config_in, buf, BUFF_SIZE);

    const char s[2] = "#";
    char *option;
    strtok(buf, s);
    option = strtok(NULL, s);//get rid of title
    while(option != NULL){
//        printf("Option: %s\n", option);

        char* value = strtok(NULL, s);
        value++;
        value[strlen(value)-1]='\0';
//        printf("Value: %s\n", value);

        if(strcmp(option, "SERVER_NAME")==0){
            c->server_name = value;
        }else if(strcmp(option, "PORT")==0){
            c->port = atoi(value);
        }else if(strcmp(option, "ROOT_FOLDER")==0){
            c->root = value;
        }else if(strcmp(option, "CONNECTIONS")==0){
            c->connections = atoi(value);
        }else if(strcmp(option, "HOME_PAGE")==0){
            c->path_home = value;
        }else if(strcmp(option, "404_FILE")==0){
            c->path_404 = value;
        }else if(strcmp(option, "CONCURRENCY")==0) {
            c->concurr_opt = strcmp(value, "THREADS") ? CONCURR_OPT_THREAD : CONCURR_OPT_PROCESS;
        }else if(strcmp(option, "BACKLOG")==0){
                c->backlog = atoi(value);
        }else{
            fprintf(stderr, "Configuration Error: %s is not a option.", option);
            exit(EXIT_FAILURE);
        }
        option = strtok(NULL, s);
    }
}

void config_print(const Config c){
    fprintf(stderr, "========= SERVER NAME: %s =========\n", c->server_name);
    fprintf(stderr, "== Port number:               %d\n", c->port);
    fprintf(stderr, "== HTML Root folder:          %s\n", c->root);
    fprintf(stderr, "== 404 page:                  %s\n", c->path_404);
    fprintf(stderr, "== Home page:                 %s\n", c->path_home);
    fprintf(stderr, "== Concurrency:               %d %s\n", c->connections, (c->concurr_opt)?"threads":"processes");
    fprintf(stderr, "== Backlog:                   %d\n", c->backlog);
    fprintf(stderr, "========= :SERVER STARTING: =========\n");
}