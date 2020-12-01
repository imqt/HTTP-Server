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

void *dealer(void *vargp);

int main(int argc, const char * argv[])
{

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
        ret = execl("/bin/file", "file", "--mime-type", "-b","libdc.so", (char *)NULL);
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
        dc_write(STDOUT_FILENO, "", 1);

            fflush(stdout);//flushall();
            // write(pipefd[1], "good-bye", 9); // null-terminated string!
            close(pipefd[1]);
            dup2(stdout_bk, fileno(stdout));//restore
            printf("this is now\n");

            char buf[101];
            read(pipefd[0], buf, 100); 
            fprintf(stderr, "got this from the pipe >>>%s<<<\n", buf);
            fprintf(stderr, "child exited, status=%d\n", WEXITSTATUS(status));
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }   

	// Need to read config stuff here
	// Initial server setup from config    

    int sfd = dc_socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, BACKLOG);

    pthread_t thread_id;

    for (int i = 0; i < DEALERS; i++) {
        pthread_create(&thread_id, NULL, dealer, (void *) &sfd);
    }
    pthread_join(thread_id, NULL); // wait for the last thread to end

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
            	respond(cfd, file_name, content_type_code);
            }
        }
        dc_close(cfd);
    }
}
