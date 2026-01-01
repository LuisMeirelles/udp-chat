//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void) {
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("An error occured when trying to start a socket");
        return errno;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    do {
        char message[1024];

        fgets(message, sizeof(message), stdin);

        sendto(fd, message, sizeof(message), 0, (const struct sockaddr *) &addr, sizeof(addr));
    } while (1);
}
