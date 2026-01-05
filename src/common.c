//
// Created by meirelles on 12/31/25.
//

#include "../include/common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    if (ip_conversion == 0) {
        fprintf(stderr, "The source string (%s) is not a valid IP address in the AF_INET family.", ip_address);
        exit(-1);
    }

    if (ip_conversion == -1) {
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

connection_config initialize_connection(const char **argv) {
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

    return (connection_config){
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

void send_from_stdin(const int fd, const struct sockaddr_in sock_addr, const size_t buffer_size) {
    ssize_t bytes_read = 0;
    char message[buffer_size];

    do {
        bytes_read = read(STDIN_FILENO, message, buffer_size);

        if (bytes_read == -1) {
            const short error = errno;
            perror("Error while trying to read bytes from stdin");

            exit(error);
        }

        if (bytes_read == 0) {
            continue;
        }

        const ssize_t bytes_sent = sendto(
            fd,
            message,
            bytes_read,
            0,
            (const struct sockaddr *) &sock_addr, sizeof(sock_addr)
        );

        if (bytes_sent == -1) {
            const short error = errno;
            perror("error while trying to send bytes to server");

            exit(error);
        }
    } while (message[bytes_read - 1] != '\n');
}

typedef struct {
    char *data;
    struct sockaddr_in address;
    size_t length;
} message;

void receive_from(const int fd, const size_t buffer_size, message *msg) {
    socklen_t addr_size = sizeof(struct sockaddr_in);

    const ssize_t bytes_recvd = recvfrom(fd, msg->data, buffer_size, 0, (struct sockaddr *) &msg->address, &addr_size);

    if (bytes_recvd == -1) {
        const short error = errno;

        perror("An error occurred while trying to receive a message from socket");
        close(fd);

        exit(error);
    }

    msg->length = bytes_recvd;
}

ssize_t update_peers(struct sockaddr_in **peers, size_t *peers_count, struct sockaddr_in **addr_recvd) {
    ssize_t peer_index = -1;

    for (size_t i = 0; i < *peers_count; i++) {
        const struct sockaddr_in peer = *peers[i];

        if (
            peer.sin_addr.s_addr == (*addr_recvd)->sin_addr.s_addr
            && peer.sin_port == (*addr_recvd)->sin_port
        ) {
            peer_index = (ssize_t) i;
            break;
        }
    }

    if (peer_index == -1) {
        peers[*peers_count] = *addr_recvd;
        (*peers_count)++;

        char readable_address[15] = {0};
        inet_ntop(AF_INET, &(*addr_recvd)->sin_addr.s_addr, readable_address, 15);

        printf("Message received from %s:%d:\n\t", readable_address, (*addr_recvd)->sin_port);
        fflush(stdout);
    }

    return peer_index;
}

void read_from_socket(const int fd, const size_t buffer_size, const size_t max_peers) {
    struct sockaddr_in *peers = calloc(max_peers, sizeof(struct sockaddr_in));
    size_t peers_count = 0;

    fwrite("<< ", 3, 1, stdout);

    do {
        char buf[buffer_size];

        message msg = {
            .data = buf,
        };

        receive_from(fd, buffer_size, &msg);

        struct sockaddr_in *addr_recvd = &msg.address;

        const size_t bytes_recvd = msg.length;

        if (bytes_recvd == 0) {
            continue;
        }

        const size_t peer_index = update_peers(&peers, &peers_count, &addr_recvd);

        fwrite(buf, bytes_recvd, 1, stdout);
        fflush(stdout);

        if (buf[bytes_recvd - 1] == '\n') {
            memset(&peers[peer_index], 0, sizeof(struct sockaddr_in));
            peers_count--;

            fwrite(">> ", 3, 1, stdout);
            fflush(stdout);

            break;
        }
    } while (1);
}
