//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "chat/common.h"

#define BUFFER_SIZE 3

int main(const int argc, const char **argv) {
    const address input_address = handle_inputs(argc, argv);

    fprintf(stderr, "Running on %s:%d\n", input_address.ip, input_address.port);

    const int socket_file_descriptor = start_socket();

    struct in_addr s_addr;

    inet_pton(AF_INET, input_address.ip, &s_addr);

    const struct sockaddr_in sock_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(input_address.port),
        .sin_addr = s_addr
    };

    do {
        char message[BUFFER_SIZE] = {0};

        const size_t read_bytes = read(STDIN_FILENO, message, BUFFER_SIZE);

        sendto(socket_file_descriptor, &message, read_bytes, 0, (const struct sockaddr *) &sock_addr, sizeof(sock_addr));
    } while (1);
}
