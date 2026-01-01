//
// Created by meirelles on 1/1/26.
//

#ifndef CHAT_RUNTIME_H
#define CHAT_RUNTIME_H

#include <netinet/in.h>

void run_client(int fd, struct sockaddr_in sock_addr, size_t buffer_size);

#endif //CHAT_RUNTIME_H