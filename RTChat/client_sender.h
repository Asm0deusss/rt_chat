#include "common.h"

int configure_socket();

int configure_sockaddr_in(struct sockaddr_in *sockaddr);

int connect_client(int socket, struct sockaddr_in sockaddr);

void send_delta(Delta delta, int server_socket);
