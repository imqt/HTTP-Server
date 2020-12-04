#include "dc_lib/sys/socket.h"
#include "dc_lib/unistd.h"

#include "server/pool.h"
#include "config/config.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <sys/mman.h>

static sem_t config_mutex;

int main(int argc, const char * argv[])
{
    Config config = mmap(NULL, sizeof(struct Config_S), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    memcpy(config, config_create(), sizeof(struct Config_S));
    config_print(config);

    int sfd = dc_socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config->port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dc_bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    dc_listen(sfd, config->backlog);

    int enable = 1;

    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR)");
    }

    fprintf(stderr, "Server running with: ");
    if (config->concurr_opt == CONCURR_OPT_THREAD) {
        fprintf(stderr, "%d threads\n", config->connections);
        threadz(sfd, config->connections);
    } else {
        fprintf(stderr, "%d processes\n", config->connections);
        processez(sfd, config->connections);
    }

    dc_close(sfd);
    return EXIT_SUCCESS;
}