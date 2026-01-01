//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
        fprintf(stderr, "An error occured when trying to start a socket");
        return errno;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = s_addr;

    do {
        char message[BUFFER_SIZE] = {0};

        const size_t read_bytes = read(STDIN_FILENO, message, BUFFER_SIZE);

        sendto(fd, &message, read_bytes, 0, (const struct sockaddr *) &addr, sizeof(addr));
    } while (1);
}
