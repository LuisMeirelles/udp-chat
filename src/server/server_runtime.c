//
// Created by meirelles on 1/1/26.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

            break;
        }
    } while (1);
}
