#include "dc_lib/sys/socket.h"
#include "dc_lib/unistd.h"
#include "http/request.h"
#include "http/response.h"
#include "shared.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define BACKLOG 5
#define DEALERS 10
#define CONF "edit-config.txt"
#define SERVER_NAME "Team-7"
#define DEFAULT_ROOT "../../rsc/"
#define DEFAULT_404 "../../rsc/404.html"
#define DEFAULT_PORT 6967 

uint16_t port_number;

void set_variables();

void *dealer(void *vargp);

void pdealer(int sfd) {
    for(;;)
    {
        // Check if server is running with processes
        // If yes, break.
        int cfd = dc_accept(sfd, NULL, NULL);
        char client_request[BUF_SIZE]; 
        ssize_t request_len; 
        while((request_len = dc_read(cfd, client_request, BUF_SIZE)) > 0)
        {
            char file_name[BUF_SIZE] = "../../rsc/";
            int content_type_code = 0;
            int request_code = parse_request(client_request, file_name, &content_type_code, request_len);
            if (request_code) {
                respond(cfd, file_name, content_type_code, request_code);
            }
        }
        dc_close(cfd);
    }
}

int main(int argc, const char * argv[])
{
  
	// Need to read config stuff here
	// Initial server setup from config    

    int sfd = dc_socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFAULT_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, BACKLOG);

    // pthread_t thread_id;

    // for (int i = 0; i < DEALERS; i++) {
    //     pthread_create(&thread_id, NULL, dealer, (void *) &sfd);
    //     // dc_write(STDOUT_FILENO, "\n//////////////////////////////After responding\n", 50);
    // }
    // pthread_join(thread_id, NULL); // wait for the last thread to end

    pid_t child_pid, wpid;
    int ret, status;
            printf("LET'S GO???\n");

    for (int i = 0; i < 5; i++) {
        child_pid = fork();
        if(child_pid == -1) { 
            perror("fork");
            exit(EXIT_FAILURE);
        } else if(child_pid == 0) { //Child
            char *aaa = "HOHO";
            dc_write(STDOUT_FILENO, aaa, strlen(aaa));
            pdealer(sfd);
            break;
        }   
    }
    do {
        char *aaa = "HAHA\n";
        dc_write(STDOUT_FILENO, aaa, strlen(aaa));
        wpid = waitpid(child_pid, &status, WUNTRACED);
        if (wpid == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "child exited, status=%d\n", WEXITSTATUS(status));
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    dc_close(sfd);

    return EXIT_SUCCESS;
}

void *dealer(void *vargp) {
    int *sfd = (int *) vargp;
    for(;;)
    {
        // Check if server is running with processes
        // If yes, break.
        int cfd = dc_accept(*sfd, NULL, NULL);
        char client_request[BUF_SIZE]; 
        ssize_t request_len; 
        while((request_len = dc_read(cfd, client_request, BUF_SIZE)) > 0)
        {
            char file_name[BUF_SIZE] = "../../rsc/";
            int content_type_code = 0;
            int request_code = parse_request(client_request, file_name, &content_type_code, request_len);
            if (request_code) {
            	respond(cfd, file_name, content_type_code, request_code);
            }
        }
        dc_close(cfd);
    }
}
