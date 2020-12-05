#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
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
    Config this = mmap(NULL, sizeof(struct Config_S), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    config_set_default(this);
    config_set_file(this);
    return this;
}

void config_delete(Config this){
    munmap(this, sizeof(struct Config_S));
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

    const char s[2] = "~";
    char *option;
    strtok(buf, s);
    option = strtok(NULL, s);//get rid of title
    while(option != NULL){
        char* value = strtok(NULL, s);
        value++;
        value[strlen(value)-1]='\0';

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
            if(strcmp(value, "THREADS")==0)
                c->concurr_opt = CONCURR_OPT_THREAD;
            else if(strcmp(value, "PROCESSESS")==0)
                c->concurr_opt = CONCURR_OPT_PROCESS;
            else{
                fprintf(stderr, "Configuration Error: %s is not a valid concurrency option.\n", value);
            }
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
    fprintf(stderr, "Change 404_PAGE, HOME_PAGE, or ROOT with format option:value\n");
}

void *config_handler(void *vargp){
    Config config = ((Config*)vargp)[0];
    sem_t* config_mutex = ((sem_t**)vargp)[1];

    for(;;){
        char buffer[256];
        while(read(STDIN_FILENO, buffer, 256)>0){
            const char s[2] = ":";
            char *option, *value;
            option = strtok(buffer, s);
            value = strtok(NULL, s);
            value[strlen(value)-1]='\0';

            sem_wait(config_mutex);
            printf("option: %s, value: %s", option, value);
            if(strcmp(option, "ROOT_FOLDER")==0){
                config->root = value;
            }else if(strcmp(option, "HOME_PAGE")==0){
                config->path_home = value;
            }else if(strcmp(option, "404_FILE")==0){
                config->path_404 = value;
            }else{
                fprintf(stderr, "Configuration Error: %s is not a changeable option.", option);
            }
            config_print(config);
            sem_post(config_mutex);
        }
    }
}