//
// Created by meirelles on 1/1/26.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void run_server(const int fd, const struct sockaddr_in bind_addr, const size_t buffer_size) {
    const int bind_status = bind(fd, (const struct sockaddr *) &bind_addr, sizeof(bind_addr));
    const short bind_error = errno;

    if (bind_status == -1) {
        perror("An error occurred while trying to bind socket");
        close(fd);

        exit(bind_error);
    }

    do {
        char buf[buffer_size];

        const ssize_t bytes_recvd = recv(fd, buf, buffer_size, 0);
        const short recv_error = errno;

        if (bytes_recvd == -1) {
            perror("An error occurred while trying to receive a message from socket");
            close(fd);

            exit(recv_error);
        }

        /// Here, we separate the packets by `/n`, but in the future we will loop through the connections (perhaps using
        /// `recv_from` and a connection pool?) so that we can detect `\0` at the end of the message.
        fwrite(buf, bytes_recvd, 1, stdout);
        fflush(stdout);
    } while (1);
}
