//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("An error occurred when trying to start a socket");
        return errno;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;

    const int connection = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));

    const short error = errno;

    if (connection == -1) {
        perror("An error occurred while trying to bind socket");
        close(fd);

        return error;
    }

    char buf[1024];

    do {
        memset(&buf, 0, 1024);
        recvfrom(fd, buf, 1024, 0, (struct sockaddr *) &addr, (unsigned int *) sizeof(addr));
        printf("%s", buf);
    } while (1);
}
