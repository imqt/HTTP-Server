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

void generate_fd_set(fd_set* fdSet, int sfd, int* proc_fds);

static void send_fd(int socket, int *fds, int n);


const size_t NUM_CHILDREN = 10;
int main(int argc, const char * argv[])
{
    //generate sockets for IPC
    static const int parentsocket = 0;
    static const int childsocket = 1;

    int socket_pairs_fd[NUM_CHILDREN][2];
    for(size_t i = 0; i < NUM_CHILDREN;i++){
        if((socketpair(AF_UNIX, SOCK_STREAM,0, socket_pairs_fd[i]))==-1){
            perror("Socket Pair");
            exit(EXIT_FAILURE);
        }
    }

    //create children
    pid_t pid;
    size_t iterator = 0;
    while(iterator++<NUM_CHILDREN){
        pid = fork();
        if(pid == 0){
            close(socket_pairs_fd[iterator][parentsocket]);
            break;
        }else{
            close(socket_pairs_fd[iterator][childsocket]);
        }
    }

    if(pid == 0){



    }else{
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

        //store all child end sockets for communication later
        int proc_fds[NUM_CHILDREN], avail_proc[NUM_CHILDREN];
        size_t avail_index = NUM_CHILDREN;
        for(size_t i = 0; i < NUM_CHILDREN; i++){
            avail_proc[i] = socket_pairs_fd[i][childsocket];
            proc_fds[i] = socket_pairs_fd[i][childsocket];
        }

        //create reference fd sets; to check if fd is a process or inet
        fd_set sfd_fds_master, proc_fds_master, merge_fds; //process sockets
        FD_ZERO(&sfd_fds_master);
        FD_SET(sfd, &sfd_fds_master);

        FD_ZERO(&proc_fds_master);
        for(size_t i = 0; i < NUM_CHILDREN; i++){
            FD_SET(proc_fds[i], &proc_fds_master);
        }

        //listen for connections; give client requests to avail child processes, add child to available
        while(1) {
            int curr_fd;
            generate_fd_set(&merge_fds, sfd, proc_fds);
            if ((curr_fd = select(NUM_CHILDREN, &merge_fds, NULL, NULL, NULL)) == -1) {
                perror("Select");
                exit(EXIT_FAILURE);
            }

            if (FD_ISSET(curr_fd, &sfd_fds_master)) {
                if(avail_index == -1){
                    //wait for a avail socket
                    int process_fd;
                    if((process_fd = select(NUM_CHILDREN, &merge_fds, NULL, NULL, NULL)) == -1) {
                        perror("Process Select");
                        exit(EXIT_FAILURE);
                    }
                    send_fd(process_fd, &curr_fd,1);
                }else{
                    send_fd(avail_proc[avail_index--], &curr_fd, 1);
                }
            }else if (FD_ISSET(curr_fd, &proc_fds_master)){
                avail_proc[avail_index++] = curr_fd;
            }
        }
    }

//    pthread_t thread_id;
//
//    for (int i = 0; i < DEALERS; i++) {
//        pthread_create(&thread_id, NULL, dealer, (void *) &sfd);
//    }
//    pthread_join(thread_id, NULL); // wait for the last thread to end
//
//    dc_close(sfd);

    return EXIT_SUCCESS;
}

void generate_fd_set(fd_set* fdSet, int sfd, int* proc_fds){
    FD_ZERO(fdSet);
    FD_SET(sfd, fdSet);
    for(size_t i = 0; i < NUM_CHILDREN; i++){
        FD_SET(proc_fds[i], fdSet);
    }
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

static void send_fd(int socket, int *fds, int n){
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(n*sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = {.iov_base = &dup, .iov_len = sizeof(dup)};

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

    memcpy((int*)CMSG_DATA(cmsg), fds, n* sizeof(int));

    if(sendmsg(socket, &msg, 0)<0){
        perror("Socket FD Message");
        exit(EXIT_FAILURE);
    }
}

static int * recv_fd(int socket, int n) {
    int *fds = malloc(n * sizeof(int));
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if (recvmsg(socket, &msg, 0) < 0){
        perror("Failed to receive message");
        exit(EXIT_FAILURE);
    }

    cmsg = CMSG_FIRSTHDR(&msg);

    memcpy(fds, (int *) CMSG_DATA(cmsg), n * sizeof(int));

    return fds;
}