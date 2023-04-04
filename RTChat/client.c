#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "TTY_manager.h"
#include <poll.h>
#include "client_sender.h"
#define PORT 8080
#define HOST "127.0.0.1"

int main(int argc, char const* argv[])
{
    int valread;
    int sock = configure_socket();
    struct sockaddr_in socket_info;

    configure_sockaddr_in(&socket_info);
    int client_fd = connect_client(sock, socket_info);

    char buffer[1024] = { 0 };
	printf("Writesome: ");
	unset_canonical();
	Delta delta = {.delta = ' '};
	while(1){
		delta.delta = getchar();
		send_delta(delta, sock);
	}
	set_canonical();
	printf("Hello message sent\n");
	close(client_fd);
	return 0;
}