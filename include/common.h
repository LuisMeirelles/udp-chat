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

connection_config initialize_connection(const char **argv);

void bind_address(int fd, struct sockaddr_in bind_addr);

void send_from_stdin(int fd, struct sockaddr_in sock_addr, size_t buffer_size);

void read_from_socket(int fd, size_t buffer_size, size_t max_peers);

#endif //CHAT_COMMON_H
