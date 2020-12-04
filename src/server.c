#include "dc_lib/sys/socket.h"
#include "dc_lib/unistd.h"

#include "server/pool.h"
#include "config/config.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, const char * argv[])
{
    Config c = config_create();
    config_print(c);

    int sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(c->port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, c->backlog);

    int enable = 1;

    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR)");
    }

    fprintf(stderr, "Server running with: ");
    if (c->concurr_opt == CONCURR_OPT_THREAD) {
        fprintf(stderr, "%d threads\n", c->connections);
        threadz(sfd, c->connections);
    } else {
        fprintf(stderr, "%d processes\n", c->connections);
        processez(sfd, c->connections);
    }

    dc_close(sfd);
    return EXIT_SUCCESS;
}