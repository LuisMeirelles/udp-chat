//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include "common.h"

#define BUFFER_SIZE 2

int main(const int argc, const char **argv) {
    const address input_address = handle_inputs(argc, argv);

    fprintf(stderr, "Running on %s:%d\n", input_address.ip, input_address.port);

    const int socket_file_descriptor = start_socket();

    const struct sockaddr_in sock_addr = get_sock_addr(input_address);

    do {
        char message[BUFFER_SIZE] = {0};

        const ssize_t bytes_read = read(STDIN_FILENO, message, BUFFER_SIZE);
        const short read_error = errno;

        if (bytes_read == -1) {
            perror("Error while trying to read bytes from stdin");
            return read_error;
        }

        const ssize_t bytes_sent = sendto(socket_file_descriptor, message, bytes_read, 0,
                                          (const struct sockaddr *) &sock_addr, sizeof(sock_addr));
        const short sendto_error = errno;

        if (bytes_sent == -1) {
            perror("error while trying to send bytes to server");
            return sendto_error;
        }
    } while (1);
}
