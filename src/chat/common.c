//
// Created by meirelles on 12/31/25.
//

#include "chat/common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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

    return (address){
        .ip = ip_address,
        .port = port
    };
}

int start_socket(void) {
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("An error occured when trying to start a socket");
        exit(errno);
    }

    return fd;
}

struct sockaddr_in get_sock_addr(const address addr) {
    struct in_addr s_addr;

    inet_pton(AF_INET, addr.ip, &s_addr);

    return (struct sockaddr_in){
        .sin_family = AF_INET,
        .sin_port = htons(addr.port),
        .sin_addr = s_addr
    };
}

connection_config initialize_connection(const int argc, const char **argv) {
    const char *in_args[3] = {
        argv[0],
        argv[1],
        argv[2]
    };

    const char *dst_args[3] = {
        argv[0],
        argv[3],
        argv[4]
    };

    const address input_address = handle_inputs(3, in_args);
    const address destination_address = handle_inputs(3, dst_args);

    const int socket_file_descriptor = start_socket();
    const int dst_socket_file_descriptor = start_socket();

    const struct sockaddr_in sock_addr = get_sock_addr(input_address);
    const struct sockaddr_in dst_addr = get_sock_addr(destination_address);

    return (connection_config) {
        .source_fd = socket_file_descriptor,
        .destination_fd = dst_socket_file_descriptor,
        .source_address = sock_addr,
        .destination_address = dst_addr,
        .ip = input_address.ip,
        .port = input_address.port
    };
}

void bind_address(const int fd, const struct sockaddr_in bind_addr) {
    const int bind_status = bind(fd, (const struct sockaddr *) &bind_addr, sizeof(bind_addr));

    if (bind_status == -1) {
        const short error = errno;

        perror("An error occurred while trying to bind socket");
        close(fd);

        exit(error);
    }
}
