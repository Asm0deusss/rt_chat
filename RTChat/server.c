// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080

typedef struct UserInRoom{
	char* adress;
} UserInRoom;

typedef struct Room{
	UserInRoom users[5];
	int num_of_connections;
} Room;


Room rooms[80];


void *process_connected_client(void* data){
	
		char *greeting = "Please enter room you want to enter: ";
		send(*(int*)data, greeting, strlen(greeting), 0);
		struct sockaddr_in user;
		struct sockaddr_in* restrict user_ptr = &user;
		int userlen = sizeof(user);
		int getpeererr = getpeername(*(int*)data, (struct sockaddr* restrict)user_ptr, &userlen);
		char ip[INET_ADDRSTRLEN];
		const char* ip_get_err = inet_ntop(AF_INET, &user_ptr->sin_addr, ip, INET_ADDRSTRLEN);
		int port = ntohs(user_ptr->sin_port);
		
		printf("%s : %d connected\n", ip, port);
	while(1){


		// char a[1];
		// int valread = read(*(int*)data, &a, 1);
		// int room_num = a[0] - '0';
		
		


		char a[1];
		int valread = read(*(int*)data, &a, 1);
		if (valread < 0){
			break;
		}
		if (a[0] == (char)127){

            fseek(stdin, -1 , 1);
                    putc(' ', stdin);
            fseek(stdin, -1 , 1);

            continue;
        }
		putchar(a[0]);
		fflush(stdout);
		if (a == "\n"){
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
	char* hello = "Hello from server";
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	for (int i = 0; i < 80; ++i){
		rooms[i].num_of_connections = 0;
	}
	// Forcefully attaching socket to the port 8080
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
		pthread_create(&new_thread, NULL, process_connected_client, &new_socket);
	}

	// // send(new_socket, hello, strlen(hello), 0);
	// printf("Hello message sent\n");

	// // closing the connected socket
	close(new_socket);
	// // closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
