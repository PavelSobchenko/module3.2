#pragma once

#include "esp_log.h"

typedef struct Logger Logger;

struct Logger {
    const char *tag;
};

void logInfo(const Logger *logger, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void logWarning(const Logger *logger, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void logError(const Logger *logger, const char *fmt, ...) __attribute__((format(printf, 2, 3)));