//
// Created by meirelles on 12/31/25.
//

#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <netinet/in.h>

typedef struct {
    int source_fd;
    int destination_fd;
    struct sockaddr_in source_address;
    struct sockaddr_in destination_address;
    const char *ip;
    uint16_t port;
} connection_config;

connection_config initialize_connection(int argc, const char **argv);

void bind_address(int fd, struct sockaddr_in bind_addr);

#endif //CHAT_COMMON_H
