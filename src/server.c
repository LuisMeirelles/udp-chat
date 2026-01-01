//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(const int argc, const char **argv) {
    const char *ip_address;
    uint16_t port = 0;

    if (argc == 1) {
        printf("address not provided, assuming `127.0.0.1`.\n");
        ip_address = "127.0.0.1";
    } else {
        ip_address = argv[1];
    }

    if (argc <= 2) {
        printf("port not provided, assuming `1234`.\n");
        port = 1234;
    } else {
        char *endptr;
        port = strtoul(argv[2], &endptr, 10);
    }

    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("An error occurred when trying to start a socket");
        return errno;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    inet_pton(AF_INET, ip_address, &addr.sin_addr.s_addr);

    const int connection = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));

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
            return recv_error;
        }

        /// Here, we separate the packets by `/n`, but in the future we will loop through the connections (perhaps using
        /// `recv_from` and a connection pool?) so that we can detect `\0` at the end of the message.
        write(STDOUT_FILENO, buf, n);
    } while (1);
}
