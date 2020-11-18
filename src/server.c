#include "dc_lib/sys/socket.h"
#include "dc_lib/unistd.h"
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

int sfd;

int parse_request(char request[]);

void *dealer(void *vargp);

int main(int argc, const char * argv[])
{
    // Create the socket
    sfd = dc_socket(AF_INET, SOCK_STREAM, 0);

    // Identify the socket
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    pthread_t thread_id;

    for (int i = 0; i < DEALERS; i++) {
        pthread_create(&thread_id, NULL, dealer, NULL);
    } // currently, only the last thread_id is stored, might need to change for later

    pthread_join(thread_id, NULL); // wait for the last thread to end

    dc_close(sfd); // Close server socket

    return EXIT_SUCCESS;
}


void *dealer(void *vargp) {
    dc_listen(sfd, BACKLOG);
    for(;;)
    {
        int cfd = dc_accept(sfd, NULL, NULL);

        // Deal with client request:
        char client_request[BUF_SIZE]; 
        ssize_t request_len; 
        while((request_len = dc_read(cfd, client_request, BUF_SIZE)) > 0)
        {
            // Print client request to stdout
            dc_write(STDOUT_FILENO, client_request, request_len);  

            // Parse the client_request for info
            int request_code = parse_request(client_request);
            // TODO: ^^
            dc_write(STDOUT_FILENO, "\nAfter parse_request\n", 23);

            if (request_code) {

                char file_name[] = "../../rsc/404.html";

                // Constuct a reponse:
                char response[4096] = ""; // TODO: change this to dynamic memory 
                construct_response(response, get_content(file_name), 200);
                dc_write(STDOUT_FILENO, "\nAFter construct_response\n", 28);

                // Send response to client
                dc_write(cfd, response, strlen(response));
                dc_write(STDOUT_FILENO, "\nAFter responding\n", 20);
                // Print to server's terminal
                dc_write(STDOUT_FILENO, response, strlen(response));
            }
        } // end while
        dc_close(cfd); // Close client socket
    } // end for
}


int parse_request(char request[]) {
    char GET_R[] = "GET";
    for (int i = 0; i < 3; i++)
        if (GET_R[i] != request[i])
            return 0; // unsupported request
    return 1; // GET request
}
