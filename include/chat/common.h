//
// Created by meirelles on 12/31/25.
//

#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <stdint.h>

typedef struct {
    const char *ip;
    uint16_t port;
} address;

address handle_inputs(int argc, const char **argv);

#endif //CHAT_COMMON_H
