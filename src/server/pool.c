//
// Created by wuviv on 2020-12-03.
//

#include <semaphore.h>
#include "pool.h"

void *dealer(void *vargp) {
    int sfd = ((int*)vargp)[0];
    int id = ((int*)vargp)[1];
    Config config = ((Config*)vargp)[2];
    sem_t* config_mutex = ((sem_t**)vargp)[3];

    for(;;){
        // Check if server is running with processes
        // If yes, break.
        int cfd = dc_accept(sfd, NULL, NULL);
        sem_wait(config_mutex);
        fprintf(stderr, "%s %d is dealing with client fd %d\n",(config->concurr_opt == CONCURR_OPT_THREAD) ? "Thread" : "Process", id+1, cfd);

        char client_request[BUF_SIZE];
        ssize_t request_len;
        while((request_len = dc_read(cfd, client_request, BUF_SIZE)) > 0)
        {
            char file_name[BUF_SIZE];
            strcat(file_name, config->root);
            int request_code = parse_request(client_request, file_name, request_len);
            if (request_code) {
                respond(cfd, file_name, request_code, config, config_mutex);
            }
        }
        sem_post(config_mutex);
        dc_close(cfd);
    }
}

void threadz(int sfd, Config config, sem_t* config_mutex) {
    pthread_t thread_id;
    int dealer_args_arr[config->connections][2];
    for (int i = 0; i < config->connections; i++) {
        dealer_args_arr[i][0] = sfd;
        dealer_args_arr[i][1] = i;
        void * args_arr[4] = {dealer_args_arr[i][0], dealer_args_arr[i][1], config, config_mutex};
        pthread_create(&thread_id, NULL, dealer, args_arr);
    }
    pthread_join(thread_id, NULL); // wait for the last thread to end
}

void processez(int sfd, Config config, sem_t* config_mutex) {
    pid_t child_pid, wpid;
    int ret, status;
    int dealer_args_arr[config->connections][2];
    for (int i = 0; i < config->connections; i++) {
        dealer_args_arr[i][0] = sfd;
        dealer_args_arr[i][1] = i;
        child_pid = fork();
        if(child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if(child_pid == 0) { //Child
            void * args_arr[4] = {dealer_args_arr[i][0], dealer_args_arr[i][1], config, config_mutex};
            dealer(args_arr);
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