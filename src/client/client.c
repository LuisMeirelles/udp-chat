//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>

#include "common.h"
#include "runtime.h"

#define BUFFER_SIZE 2

int main(const int argc, const char **argv) {
    const address input_address = handle_inputs(argc, argv);

    fprintf(stderr, "Running on %s:%d\n", input_address.ip, input_address.port);

    const int socket_file_descriptor = start_socket();

    const struct sockaddr_in sock_addr = get_sock_addr(input_address);

    run_client(socket_file_descriptor, sock_addr, BUFFER_SIZE);
}
