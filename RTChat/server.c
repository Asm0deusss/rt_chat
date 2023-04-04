#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080



void *reciever(void* listener){
	
	struct sockaddr_in user;
	struct sockaddr_in* restrict user_ptr = &user;
	int userlen = sizeof(user);
	int getpeererr = getpeername(*(int*)listener, (struct sockaddr* restrict)user_ptr, &userlen);
	char ip[INET_ADDRSTRLEN];
	const char* ip_get_err = inet_ntop(AF_INET, &user_ptr->sin_addr, ip, INET_ADDRSTRLEN);
	int port = ntohs(user_ptr->sin_port);
	printf("%s : %d connected\n", ip, port);
	while(1){
		Delta a[1];
		int valread = read(*(int*)listener, &a, sizeof(a));
		if (valread < 0){
			break;
		}
		if (a->delta == 127){
			printf("AHAHA TVOI PAPA STROIMATERIAL\n");
		}
		putchar(a[0].delta);
		fflush(stdout);
		if (a->delta == '\n'){
			break;
		}
	}
	pthread_exit(0);
}

int main(int argc, char const* argv[])
{

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while(1){
		if ((new_socket
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		pthread_t new_thread;
		pthread_create(&new_thread, NULL, reciever, &new_socket);
	}

	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
