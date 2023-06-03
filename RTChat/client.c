// Client side C/C++ program to demonstrate Socket
// programming
#include "contin_writer.h"
#include <arpa/inet.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[]) {

    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
                             sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Choose room:");
    char room_number[2];
    scanf("%s", room_number);
    send(sock, &room_number, 2, 0);
    printf("You choose room number %s\n", room_number);

    set_keypress();

    char a = ' ';
    struct pollfd pfds[1];
    pfds[0].fd = sock;
    pfds[0].events = POLLIN;

    while (1) {
        a = getchar();
        send(sock, &a, strlen(&a), 0);
        valread = poll(pfds, 1, 10);
        if (valread != 0) {
            int read_err = recv(sock, buffer, 1024, 0);
            // putchar('\b');
            printf("%s", buffer);
        }
    }

    reset_keypress();

    close(client_fd);
    return 0;
}
