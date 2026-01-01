//
// Created by meirelles on 12/31/25.
//

#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <netinet/in.h>

typedef struct {
    const char *ip;
    uint16_t port;
} address;

typedef struct {
    int fd;
    struct sockaddr_in socket_address;
    const char *ip;
    uint16_t port;
} connection_config;

address handle_inputs(int argc, const char **argv);

int start_socket(void);

struct sockaddr_in get_sock_addr(address addr);

connection_config initialize_connection(int argc, const char **argv);

#endif //CHAT_COMMON_H
