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

#define BACKLOG 5

int parse_request(char request[]) {
    char GET_R[] = "GET";
    for (int i = 0; i < 3; i++)
        if (GET_R[i] != request[i])
            return 0; // unsupported request
    return 1; // GET request
}

int main(int argc, const char * argv[])
{
    // Create the socket
    int sfd = dc_socket(AF_INET, SOCK_STREAM, 0);

    // Identify the socket
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    // Wait for incoming connection
    dc_listen(sfd, BACKLOG);
    
    for(;;)
    {
        // Getting client fd
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

            if (request_code) {

                char file_name[] = "../../rsc/404.html";

                // Constuct a reponse:
                char response[4096] = ""; // TODO: change this to dynamic memory 
                construct_response(response, get_content(file_name), 200);

                // Send response to client
                dc_write(cfd, response, strlen(response));
                dc_write(STDOUT_FILENO, "\nC\n", 3);
                // Print to server's terminal
                dc_write(STDOUT_FILENO, response, strlen(response));
            }
  
        }

        dc_close(cfd); // Close client socket
    }
    dc_close(sfd); // Close server socket

    return EXIT_SUCCESS;
}
