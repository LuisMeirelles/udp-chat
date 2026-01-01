//
// Created by meirelles on 1/1/26.
//

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void run_server(const int fd, const struct sockaddr_in bind_addr, const size_t buffer_size, const size_t max_peers) {
    struct sockaddr_in peers[max_peers];

    memset(peers, 0, sizeof(peers));

    size_t peers_count = 0;

    const int bind_status = bind(fd, (const struct sockaddr *) &bind_addr, sizeof(bind_addr));

    if (bind_status == -1) {
        const short error = errno;

        perror("An error occurred while trying to bind socket");
        close(fd);

        exit(error);
    }

    do {
        char buf[buffer_size];

        struct sockaddr_in addr_recvd;
        socklen_t addr_size;

        const ssize_t bytes_recvd = recvfrom(fd, buf, buffer_size, 0, (struct sockaddr *) &addr_recvd, &addr_size);

        if (bytes_recvd == -1) {
            const short error = errno;

            perror("An error occurred while trying to receive a message from socket");
            close(fd);

            exit(error);
        }

        ssize_t peer_index = -1;

        for (size_t i = 0; i < peers_count; i++) {
            const struct sockaddr_in peer = peers[i];

            if (
                peer.sin_addr.s_addr == addr_recvd.sin_addr.s_addr
                && peer.sin_port == addr_recvd.sin_port
            ) {
                peer_index = (ssize_t) i;
                break;
            }
        }

        char a[15] = {0};
        inet_ntop(AF_INET, &addr_recvd.sin_addr.s_addr, a, 15);

        if (peer_index == -1) {
            peers[peers_count] = addr_recvd;
            peers_count++;

            printf("Message received from %s:%d:\n\t", a, addr_recvd.sin_port);
        }

        if (buf[buffer_size - 1 > 0 ? buffer_size - 1 : buffer_size] == '\n') {
            memset(&peers[peer_index], 0, sizeof(struct sockaddr_in));
            peers_count--;
        }

        /// Here, we separate the packets by `/n`, but in the future we will loop through the connections (perhaps using
        /// `recv_from` and a connection pool?) so that we can detect `\0` at the end of the message.
        fwrite(buf, bytes_recvd, 1, stdout);
        fflush(stdout);
    } while (1);
}
