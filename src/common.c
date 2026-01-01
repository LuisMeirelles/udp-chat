//
// Created by meirelles on 12/31/25.
//

#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

typedef struct {
    const char *ip;
    uint16_t port;
} address;

address handle_inputs(const int argc, const char **argv) {
    const char *ip_address;

    uint16_t port = 0;

    if (argc == 1) {
        ip_address = "127.0.0.1";
    } else {
        ip_address = argv[1];
    }


    struct in_addr s_addr;
    const int ip_conversion = inet_pton(AF_INET, ip_address, &s_addr);

    switch (ip_conversion) {
        case 0:
            fprintf(stderr, "The source string (%s) is not a valid IP address in the AF_INET family.", ip_address);
            exit(-1);
        case -1:
            perror("inet_pton");
            exit(-1);
    }

    if (argc <= 2) {
        port = 1234;
    } else {
        char *endptr;
        port = strtoul(argv[2], &endptr, 10);

        if (*endptr != '\0') {
            fprintf(stderr, "short integer not valid. conversion from string to short failed");
            exit(-1);
        }
    }

    return (address) {
        .ip = ip_address,
        .port = port
    };
}

int start_socket(void) {
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        fprintf(stderr, "An error occured when trying to start a socket");
        return errno;
    }

    return fd;
}

#endif //CHAT_COMMON_H
