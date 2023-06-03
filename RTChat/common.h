#pragma once
#include <stdio.h>

typedef struct {
    char delta;
} Delta;

enum Errors {
    Success,
    SocketCreation,
    InvalidAddress,
    InvalidPort,
    ConnectionFaled,
    Bind,
    Listen,
    Accept,
};

typedef struct {
    enum Errors error;
    const char *message;
} ErrorMessage;

typedef struct Room {
    int users[2];
    int num_of_connections;
} Room;
