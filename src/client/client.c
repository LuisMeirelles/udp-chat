//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>

#include "chat/common.h"
#include "client/runtime.h"

#define BUFFER_SIZE 2

int main(const int argc, const char **argv) {
    const connection_config config = initialize_connection(argc, argv);

    fprintf(stderr, "Connecting to %s:%d\n", config.ip, config.port);

    run_client(config.fd, config.socket_address, BUFFER_SIZE);
}
