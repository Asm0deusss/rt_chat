#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client_sender.h"
#include "TTY_manager.h"
#include <poll.h>
#define PORT 8080
#define HOST "127.0.0.1"

int configure_socket(){
    int sock = 0;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	return sock;
}

int configure_sockaddr_in(struct sockaddr_in *sockaddr){
    sockaddr->sin_family = AF_INET;
	sockaddr->sin_port = htons(PORT);
	if (inet_pton(AF_INET, HOST, &sockaddr->sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}
}

int connect_client(int sock, struct sockaddr_in sock_info){
    int client_fd = 0;
    if ((client_fd
            = connect(sock, (struct sockaddr*)&sock_info,
                    sizeof(sock_info)))
            < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }
    return client_fd;
}

void send_delta(Delta delta, int server_socket){
    send(server_socket, &delta, sizeof(&delta), 0);
}