//
// Created by meirelles on 12/31/25.
//

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("An error occured when trying to start a socket");
        close(fd);
        return errno;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;

    const int connection = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));

    if (connection == -1) {
        perror("An error occurred while trying to bind socket");
        close(fd);
        return errno;
    }

    char buf[1024] = {0};

    recvfrom(fd, buf, 1024, 0, (struct sockaddr *) &addr, (unsigned int *) sizeof(addr));

    close(fd);

    printf("%s", buf);

    return 0;
}
