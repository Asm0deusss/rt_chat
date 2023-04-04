#include "file_manager.h"
#include <errno.h>
#include <unistd.h>

#include <sys/stat.h>

int shift_pos(int pos, int user) {
    return pos + USER_SIZE * user;
}

FILE *create_room(const char *room_name) {
    FILE *room;
    room = fopen(room_name, "w");

    if (!room) {
        perror("Error");
    }

    return room;
}

bool check_room(const char *room_name) {
    struct stat buffer;
    return (access(room_name, F_OK) == 0);
}

void commit(FILE *cur_file, int pos, Delta delta) {
    char ch;
    fseek(cur_file, pos * sizeof(ch), SEEK_SET);
    fputc(delta.delta, cur_file);
    fclose(cur_file);
}
