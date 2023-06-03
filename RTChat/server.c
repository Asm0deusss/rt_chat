#include "file_manager.h"
#include "logger.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

Room rooms[80];

void send_to_users(char ch, struct Room room) {
    dprintf(room.users[0], "%c", ch);
    dprintf(room.users[1], "%c", ch);
}

void *process_connected_client(void *data) {

    int fd = *(int *)data;

    struct sockaddr_in user;
    struct sockaddr_in *restrict user_ptr = &user;

    int userlen = sizeof(user);
    int getpeererr = getpeername(*(int *)data,
                                 (struct sockaddr *restrict)user_ptr, &userlen);
    char ip[INET_ADDRSTRLEN];
    const char *ip_get_err =
        inet_ntop(AF_INET, &user_ptr->sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(user_ptr->sin_port);

    char buf[1024];
    sprintf(buf, "%s : %d connected\n", ip, port);
    log_info(buf);

    char input_room[2];
    read(fd, input_room, 2);
    input_room[2] = 0;

    int room_number = strtol(input_room, NULL, 10);

    if (rooms[room_number].num_of_connections == 0) {
        char room_name[11];
        snprintf(room_name, 11, "./rooms/%d", room_number);
        log_info("Room has been created");
        rooms[room_number].users[0] = fd;
        rooms[room_number].num_of_connections = 1;
    } else if (rooms[room_number].num_of_connections == 1) {
        log_info("Have room");
        rooms[room_number].users[1] = fd;
        rooms[room_number].num_of_connections = 2;
    }

    while (1) {
        char cur_char;
        int valread = read(fd, &cur_char, 1);
        if (valread < 0) {
            break;
        }

        if (cur_char == 127) {
            send_to_users('\b', rooms[room_number]);

            send_to_users(' ', rooms[room_number]);

            send_to_users('\b', rooms[room_number]);
            continue;
        }

        send_to_users(cur_char, rooms[room_number]);
    }

    pthread_exit(0);
}

int main(int argc, char const *argv[]) {

    log_open("./log.log");
    log_info("Started server");

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_error("Socker error");
        log_close();
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        log_error("SetSockPort error");
        log_close();
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    for (int i = 0; i < 80; ++i) {
        rooms[i].num_of_connections = 0;
    }

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        log_error("Bind failed");
        log_close();
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            log_error("Accept failed");
            log_close();
            exit(EXIT_FAILURE);
        }
        pthread_t new_thread;
        pthread_create(&new_thread, NULL, process_connected_client,
                       &new_socket);
    }

    // // closing the connected socket
    close(new_socket);
    // // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);

    log_info("Shutting down the server");
    log_close();
    return 0;
}
