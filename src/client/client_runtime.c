//
// Created by meirelles on 1/1/26.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void run_client(const int fd, const struct sockaddr_in sock_addr, const size_t buffer_size) {
    do {
        char message[buffer_size];

        const ssize_t bytes_read = read(STDIN_FILENO, message, buffer_size);

        switch (bytes_read) {
            case -1:
                const short error = errno;
                perror("Error while trying to read bytes from stdin");

                exit(error);
            case 0:
                continue;
        }

        const ssize_t bytes_sent = sendto(fd, message, bytes_read, 0,
                                          (const struct sockaddr *) &sock_addr, sizeof(sock_addr));

        if (bytes_sent == -1) {
            const short error = errno;
            perror("error while trying to send bytes to server");

            exit(error);
        }
    } while (1);
}
