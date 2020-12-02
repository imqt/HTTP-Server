#include "response.h"
#include <stdlib.h>

char textPlain[] = "text/plain";
char textHTML[]   = "text/html";
char imgPNG[]     = "image/png";
char imgWEBP[]     = "image/webp";
char audioMPEG[]  = "audio/mpeg";
char favicon[]  = "image/webp";

void respond(int cfd, char * file_name, int content_type_code, int request_code) {
            // dc_write(STDOUT_FILENO, "\n", 1);

    char response[BUF_SIZE] = "";
    if (strlen(file_name) <= 11) {
        construct_response(response, get_content_length("../../rsc/index.html"), 200, content_type_code, "../../rsc/index.html");
        dc_write(cfd, response, strlen(response));
        if (request_code == 5) { send_content("../../rsc/index.html", cfd); }
    } 
    else if (realpath(file_name, NULL)!= NULL) {
        // Constuct a reponse when FILE EXISTS:
        construct_response(response, get_content_length(file_name), 200, content_type_code, file_name);
        // Send response to client
        dc_write(cfd, response, strlen(response));
        // Print to server's terminal
        // dc_write(STDOUT_FILENO, response, strlen(response));
        if (request_code == 5) { send_content(file_name, cfd); }
    } 

    else {
        // TODO: differentiate between handling html not found and videos/images not found
        construct_response(response, get_content_length("../../rsc/404.html"), 404, content_type_code, "../../rsc/404.html");
        // Send response to client
        dc_write(cfd, response, strlen(response));
        // Print to server's terminal
        // dc_write(STDOUT_FILENO, response, strlen(response));
        if (request_code == 5) { send_content("../../rsc/404.html", cfd); }
    }
    // dc_write(STDOUT_FILENO, "\n//////////////////////////////After responding\n", 50);
        // dc_write(STDOUT_FILENO, "\n", 1);
}

void construct_head(char response[], char *content_length, int status_code, int content_type_code, char file_name[]){
    char httpver[] = "HTTP/1.0 ";
    char status_reason[100];
    get_reason(status_reason, status_code);
    char content_type[] = "\r\nContent-Type: ";
    char content_l[] = "\r\nContent-Length: ";
    strcat(response, httpver);
    strcat(response, status_reason);
    strcat(response, content_type);
    switch(content_type_code) {
    case 0:
    strcat(response, textHTML); break;
    case 1:
    strcat(response, imgWEBP); break;
    case 2:
    strcat(response, audioMPEG); break;
    case 3:
    strcat(response, favicon); break;
    default:
    strcat(response, textPlain); break;
    }
    // char ctt[BUF_SIZE];
    // get_content_type(file_name, ctt);
    // strcat(response, ctt);
    strcat(response, content_l);
    strcat(response, content_length);
    strcat(response, "\r\n\r\n");
}
//void construct_head(char response[], char *content_length, int status_code, int content_type_code, char file_name[]) {
//    char httpver[] = "HTTP/1.0 ";
//    char status_reason[100];
//    get_reason(status_reason, status_code);
//    char content_type[] = "\r\nContent-Type: ";
//    char content_l[] = "\r\nContent-Length: ";
//    strcat(response, httpver);
//    strcat(response, status_reason);
//    strcat(response, content_type);
//    char ctt[BUF_SIZE];
//    get_content_type(file_name, ctt);
//    strcat(response, ctt);
//    strcat(response, content_l);
//    strcat(response, content_length);
//    strcat(response, "\r\n\r\n");
//}


void construct_response(char response[], char *content_length, int status_code, int content_type_code, char file_name[]) {
    construct_head(response, content_length, status_code, content_type_code, file_name);
    free(content_length);
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

char * get_content_length( char file_name[]) {
    char * len;
    long length;
    FILE * f = fopen(file_name, "rb");
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        len = malloc(8);
        snprintf(len, 8, "%ld", length);
    fclose(f);
    return len;
}

void get_content_type(char* file_name,char *content_type) {
    pid_t child_pid, wpid;
    int ret, status;

    int  stdout_bk; //is fd for stdout backup
    fprintf(stderr,"this is before redirection\n");
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
    // dc_write(STDOUT_FILENO, "", 1);
    fflush(stdout);//flushall();
    // write(pipefd[1], "good-bye", 9); // null-terminated string!
    close(pipefd[1]);
    dup2(stdout_bk, fileno(stdout));//restore
    char buf[101];
    read(pipefd[0], buf, 100);
    fprintf(stderr, "got this from the pipe >>>%s<<<\n", buf);
    strncpy(content_type, buf, strlen(buf)-1);
}

void send_content(char file_name[], int cfd) {
    uint8_t byte;
        struct stat st;
        int fd = open(file_name, O_RDONLY);
        stat(file_name, &st);
    ssize_t size = dc_read(fd, &byte, 1);
    while (size > 0) {
        dc_write(cfd, &byte, 1);
        size = dc_read(fd, &byte, 1);
    }
    close(fd);
}

