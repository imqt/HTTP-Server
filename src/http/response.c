#include "response.h"
#include "../config/config.h"
#include <stdlib.h>
#include <semaphore.h>

void respond(int cfd, char * file_name, int request_code, Config config, sem_t* config_mutex) {
    sem_wait(config_mutex);
    char response[BUF_SIZE] = "";
    char * rp;
    if (strlen(file_name) <= 11) {
        char file_index[BUF_SIZE];
        strcat(file_index, config->root);
        strcat(file_index, "index.html");
        construct_response(response, get_content_length(file_index), 200, file_index);
        dc_write(cfd, response, strlen(response));
        if (request_code == 5) { send_content(file_index, cfd); }
    } 
    else if ((rp = realpath(file_name, NULL))!= NULL) {
        // Constuct a reponse when FILE EXISTS:
        construct_response(response, get_content_length(file_name), 200, file_name);
        // Send response to client
        dc_write(cfd, response, strlen(response));
        // Print to server's terminal
        if (request_code == 5) { send_content(file_name, cfd); }
        free(rp);
    } else {
        char file_404[BUF_SIZE];
        strcat(file_404, config->root);
        strcat(file_404, "404.html");
        construct_response(response, get_content_length(file_404), 404, file_404);
        // Send response to client
        dc_write(cfd, response, strlen(response));
        // Print to server's terminal
        if (request_code == 5) { send_content(file_404, cfd); }
        free(rp);
    }
    sem_post(config_mutex);
}


void construct_response(char response[], size_t content_length, int status_code, char file_name[]) {
    construct_head(response, content_length, status_code, file_name);
}

void construct_head(char response[], size_t content_length, int status_code, char file_name[]) {
    char httpver[] = "HTTP/1.0 ";
    char status_reason[100];
        get_reason(status_reason, status_code);
    char content_type[] = "\r\nContent-Type: ";
    char content_l[] = "\r\nContent-Length: ";
    strcat(response, httpver);
    strcat(response, status_reason);
    strcat(response, content_type);
    char ctt[BUF_SIZE];
    get_content_type(file_name, ctt);
    strcat(response, ctt);
    strcat(response, content_l);
    char len[9];
    snprintf(len, 8, "%ld", content_length);

    strcat(response, len);
    strcat(response, "\r\n\r\n");
}

void get_reason(char dest[], int status_code) {
    switch(status_code) {
        case 200:
            strcpy(dest, "200 OK"); break;
        case 400: 
            strcpy(dest, "400 Bad Request"); break;
        case 404:  // html not found
            strcpy(dest, "404 Not Found"); break; 
        case 501:  // Requested method not implemented (POST, PUT, TRACE)
            strcpy(dest, "501 Not Implemented"); break;
        default:
            strcpy(dest, ""); break;
    }
}

size_t get_content_length( char file_name[]) {
    struct stat st;
    stat(file_name, &st);
    return st.st_size;
}

void get_content_type(char file_name[],char *content_type) {
    pid_t child_pid, wpid;
    int ret, status;
    int  stdout_bk; //is fd for stdout backup
    // Start redirection process
    stdout_bk = dup(fileno(stdout));
    int pipefd[2];
    pipe2(pipefd, 0); // O_NONBLOCK);
    // What used to be stdout will now go to the pipe.
    dup2(pipefd[1], fileno(stdout));

    child_pid = fork();
    if(child_pid == -1) { 
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(child_pid == 0) { //Child
        ret = execl("/bin/file", "file", "--mime-type", "-b", file_name, (char *)NULL);
        if (ret == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else {  // Parent
        do {
            wpid = waitpid(child_pid, &status, WUNTRACED);
            if (wpid == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "child exited, status=%d\n", WEXITSTATUS(status));
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } 
    fflush(stdout);//flushall();
    close(pipefd[1]);
    dup2(stdout_bk, fileno(stdout));//restore
    // back to no redirection
    char buf[101];
    read(pipefd[0], buf, 100); 
    char *ct, *ctnewline;
    ctnewline = strtok(buf, "\n");
    ct = strtok(ctnewline, " ");
    fprintf(stderr, "Content type: >>>%s<<<\n", buf);
    strncpy(content_type, ct, strlen(ct));
}

void send_content(char file_name[], int cfd) {
    uint8_t byte;
    int fd = open(file_name, O_RDONLY);
    ssize_t size = dc_read(fd, &byte, 1);
    while (size > 0) {
        dc_write(cfd, &byte, 1);
        size = dc_read(fd, &byte, 1);
    }
    close(fd);
}

