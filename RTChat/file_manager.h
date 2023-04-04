#pragma once

#include "common.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int first_pos;
    int second_pos;
    const char *file_name;
} ServerFileState;

typedef struct {
    int pos;
    const char *file_name;
} UserFileState;

enum {
    USER_SIZE = 600,
};

int shift_pos(int pos, int user);

FILE *create_room(const char *name);

bool check_room(const char *name);

void commit(FILE *file_name, int pos, Delta delta);
