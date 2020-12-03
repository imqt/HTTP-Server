#include "dc_lib/sys/socket.h"
#include "dc_lib/unistd.h"

#include "server/pool.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BACKLOG 5
#define DEALERS 11
#define CONF "edit-config.txt"
#define SERVER_NAME "Team-7"
#define DEFAULT_ROOT "../../rsc/"
#define DEFAULT_404 "../../rsc/404.html"
#define DEFAULT_INDEX "../../rsc/index.html"
// #define DEFAULT_PORT 49512

uint16_t port_number;

int thread = 0;

void set_variables();

int main(int argc, const char * argv[])
{
  
    fprintf(stderr, "========= SERVER NAME: %s =========\n", SERVER_NAME);
    fprintf(stderr, "== Port number:               %d\n", DEFAULT_PORT);
    fprintf(stderr, "== HTML Root folder:          %s\n", DEFAULT_ROOT);
    fprintf(stderr, "== 404 page:                  %s\n", DEFAULT_404);
    fprintf(stderr, "== Home page:                 %s\n", DEFAULT_INDEX);
    fprintf(stderr, "== Processes or threads:      ");
    if (thread)
        fprintf(stderr, "threads.\n");
    else
        fprintf(stderr, "processes.\n");
    fprintf(stderr, "== Max concurrent clients:    %d\n", DEALERS);

    fprintf(stderr, "========= :SERVER STARTING: =========\n");

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

    int enable = 1;

    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR)");
    }

    fprintf(stderr, "Server running with: ");
    if (thread) {
        fprintf(stderr, "%d threads\n", DEALERS);
        threadz(sfd, DEALERS);
    } else {
        fprintf(stderr, "%d processes\n", DEALERS);
        processez(sfd, DEALERS);
    }

    dc_close(sfd);
    return EXIT_SUCCESS;
}