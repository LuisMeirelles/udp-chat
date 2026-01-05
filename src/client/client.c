//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "chat/common.h"
#include "client/runtime.h"
#include "server/runtime.h"

#define BUFFER_SIZE 2
#define MAX_PEERS 5

int main(const int argc, const char **argv) {
    const connection_config config = initialize_connection(argc, argv);

    bind_address(config.source_fd, config.source_address);

    fprintf(stderr, "Connecting to %s:%d\n", config.ip, config.port);

    fd_set read_fd_set;

    struct timeval tv = {
        .tv_sec = 3600,
        .tv_usec = 0,
    };

    do {
        FD_ZERO(&read_fd_set);

        FD_SET(STDIN_FILENO, &read_fd_set);
        FD_SET(config.source_fd, &read_fd_set);

        // ReSharper disable once CppDFAEndlessLoop
        const int retval = select(config.source_fd + 1, &read_fd_set, NULL, NULL, &tv);

        switch (retval) {
            case -1:
                perror("select()");
                break;
            case 0:
                printf("No data within 30 seconds.\n");
                break;
            default:
                printf("Data is available now.\n");

                if (FD_ISSET(STDIN_FILENO, &read_fd_set)) {
                    run_client(config.destination_fd, config.destination_address, BUFFER_SIZE);
                } else if (FD_ISSET(config.source_fd, &read_fd_set)) {
                    run_server(config.source_fd, BUFFER_SIZE, MAX_PEERS);
                }

                break;
        }

        // ReSharper disable once CppDFAEndlessLoop
    } while (1);
}
