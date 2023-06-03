#include "logger.h"

#include <stdio.h>
#include <time.h>

char *log_level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

FILE *log_file = NULL;

int log_open(const char *filename) {
    log_file = fopen(filename, "w");
    if (log_file == NULL) {
        return -1;
    }
    return 0;
}

void log_close() {
    if (log_file != NULL) {
        fclose(log_file);
    }
}

void log_message(LogLevel level, const char *message) {
    time_t now;
    struct tm *now_local;
    char time_str[32];
    char log_str[MAX_LOG_LENGTH];

    now = time(NULL);
    now_local = localtime(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", now_local);

    snprintf(log_str, sizeof(log_str), "[%s] <%s>: %s\n", time_str,
             log_level_names[level], message);

    printf("%s", log_str);

    if (log_file != NULL) {
        fprintf(log_file, "%s", log_str);
        fflush(log_file);
    }
}

void log_debug(const char *message) {
    log_message(LOG_LEVEL_DEBUG, message);
}

void log_info(const char *message) {
    log_message(LOG_LEVEL_INFO, message);
}

void log_warning(const char *message) {
    log_message(LOG_LEVEL_WARNING, message);
}

void log_error(const char *message) {
    log_message(LOG_LEVEL_ERROR, message);
}