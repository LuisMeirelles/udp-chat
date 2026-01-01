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

#define BUFFER_SIZE 3

int main(const int argc, const char **argv) {
    const char *ip_address;
    struct in_addr s_addr;

    uint16_t port = 0;

    if (argc == 1) {
        ip_address = "127.0.0.1";
    } else {
        ip_address = argv[1];
    }

    const int ip_conversion = inet_pton(AF_INET, ip_address, &s_addr);

    switch (ip_conversion) {
        case 0:
            fprintf(stderr, "The source string (%s) is not a valid IP address in the AF_INET family.", ip_address);
            return -1;
        case -1:
            perror("inet_pton");
            return -1;
    }

    if (argc <= 2) {
        port = 1234;
    } else {
        char *endptr;
        port = strtoul(argv[2], &endptr, 10);

        if (*endptr != '\0') {
            fprintf(stderr, "short integer not valid. conversion from string to short failed");
            return -1;
        }
    }

    fprintf(stderr, "Running on %s:%d\n", ip_address, port);

    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        fprintf(stderr, "An error occurred when trying to start a socket");
        return errno;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = s_addr;

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
            close(fd);

            return recv_error;
        }

        /// Here, we separate the packets by `/n`, but in the future we will loop through the connections (perhaps using
        /// `recv_from` and a connection pool?) so that we can detect `\0` at the end of the message.
        fwrite(buf, n, 1, stdout);
        fflush(stdout);
    } while (1);
}
