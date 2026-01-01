//
// Created by meirelles on 12/31/25.
//

#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <netinet/in.h>

typedef struct {
    int fd;
    struct sockaddr_in socket_address;
    const char *ip;
    uint16_t port;
} connection_config;

connection_config initialize_connection(int argc, const char **argv);

#endif //CHAT_COMMON_H
