//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "common.h"

#define BUFFER_SIZE 2
#define MAX_PEERS 5

int main(const int argc, const char **argv) {
    (void) argc;

    const connection_config config = initialize_connection(argv);

    bind_address(config.source_fd, config.source_address);

    fprintf(stderr, "Connecting to %s:%d\n", config.ip, config.port);

    fd_set read_fd_set;

    do {
        // ReSharper disable once CppDFAEndlessLoop
        fwrite(">> ", 3, 1, stdout);
        fflush(stdout);

        FD_ZERO(&read_fd_set);

        FD_SET(STDIN_FILENO, &read_fd_set);
        FD_SET(config.source_fd, &read_fd_set);

        // ReSharper disable once CppDFAEndlessLoop
        const int retval = select(config.source_fd + 1, &read_fd_set, NULL, NULL, NULL);

        if (retval == -1) {
            perror("select()");
        } else {
            if (FD_ISSET(STDIN_FILENO, &read_fd_set)) {
                send_from_stdin(config.destination_fd, config.destination_address, BUFFER_SIZE);
            } else if (FD_ISSET(config.source_fd, &read_fd_set)) {
                read_from_socket(config.source_fd, BUFFER_SIZE, MAX_PEERS);
            }
        }

        // ReSharper disable once CppDFAEndlessLoop
    } while (1);
}
