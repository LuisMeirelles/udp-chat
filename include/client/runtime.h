//
// Created by meirelles on 1/1/26.
//

#ifndef CHAT_CLIENT_RUNTIME_H
#define CHAT_CLIENT_RUNTIME_H

#include <netinet/in.h>

void send_from_stdin(int fd, struct sockaddr_in sock_addr, size_t buffer_size);

#endif //CHAT_CLIENT_RUNTIME_H