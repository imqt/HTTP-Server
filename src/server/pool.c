//
// Created by wuviv on 2020-12-03.
//

#include "pool.h"

#define THREAD 0

void *dealer(void *vargp) {
    int *dargs[2];
    *dargs = (int *)vargp;
    for(;;)
    {
        // Check if server is running with processes
        // If yes, break.
        int cfd = dc_accept(*dargs[0], NULL, NULL);
        fprintf(stderr, (THREAD) ? "Thread " : "Process ");
        fprintf(stderr, "%d", dargs[1]);
        fprintf(stderr, " is dealing with client fd %d\n", cfd);
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

void threadz(int sfd, int n) {
    int dealer_args[2];
    dealer_args[0] = sfd;
    pthread_t thread_id;
    for (int i = 0; i < n; i++) {
        dealer_args[1] = i;
        pthread_create(&thread_id, NULL, dealer, (void *) dealer_args);
        // dc_write(STDOUT_FILENO, "\n//////////////////////////////After responding\n", 50);
    }
    pthread_join(thread_id, NULL); // wait for the last thread to end
}

void processez(int sfd, int n) {
    pid_t child_pid, wpid;
    int ret, status;
    for (int i = 0; i < n; i++) {
        child_pid = fork();
        if(child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if(child_pid == 0) { //Child
            dealer(&sfd);
            break;
        }
    }
    do {
        wpid = waitpid(child_pid, &status, WUNTRACED);
        if (wpid == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "child exited, status=%d\n", WEXITSTATUS(status));
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}