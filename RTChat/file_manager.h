#pragma once

#include "common.h"

#include <stdbool.h>
#include <stdio.h>

enum {
    USER_SIZE = 600,
};

FILE *create_room(const char *name);

bool check_room(const char *name);

// void send_to_users(char ch, struct Room room);