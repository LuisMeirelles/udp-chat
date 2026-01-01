//
// Created by meirelles on 1/1/26.
//

#ifndef CHAT_RUNTIME_H
#define CHAT_RUNTIME_H

#include <netinet/in.h>

void run_server(int fd, struct sockaddr_in bind_addr, size_t buffer_size, size_t max_peers);

#endif //CHAT_RUNTIME_H