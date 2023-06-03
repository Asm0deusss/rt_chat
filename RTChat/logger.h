#pragma once

#include <stdio.h>
#include <time.h>

#define MAX_LOG_LENGTH 1024

typedef enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3
} LogLevel;

int log_open(const char *filename);

void log_close();

void log_message(LogLevel level, const char *message);

void log_debug(const char *message);

void log_info(const char *message);

void log_warning(const char *message);

void log_error(const char *message);