//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "chat/common.h"

#define BUFFER_SIZE 3

int main(const int argc, const char **argv) {
    const address input_address = handle_inputs(argc, argv);

    fprintf(stderr, "Running on %s:%d\n", input_address.ip, input_address.port);

    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        fprintf(stderr, "An error occurred when trying to start a socket");
        return errno;
    }

    struct in_addr s_addr;

    inet_pton(AF_INET, input_address.ip, &s_addr);

    const struct sockaddr_in sock_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(input_address.port),
        .sin_addr = s_addr
    };

    const int connection = bind(fd, (const struct sockaddr *) &sock_addr, sizeof(sock_addr));

    const short bind_error = errno;

    if (connection == -1) {
        perror("An error occurred while trying to bind socket");
        close(fd);

        return bind_error;
    }

    do {
        char buf[BUFFER_SIZE] = {0};

        const ssize_t n = recv(fd, buf, BUFFER_SIZE, 0);
        const short recv_error = errno;

        if (n == -1) {
            perror("An error occurred while trying to receive a message from socket");
            close(fd);

            return recv_error;
        }

        /// Here, we separate the packets by `/n`, but in the future we will loop through the connections (perhaps using
        /// `recv_from` and a connection pool?) so that we can detect `\0` at the end of the message.
        fwrite(buf, n, 1, stdout);
        fflush(stdout);
    } while (1);
}
