#include <stdio.h>
#include "logger.h"

#define LOG_BUF_SIZE 128

static void logV(const Logger *logger, esp_log_level_t level, const char *fmt, va_list args) {
    char buf[LOG_BUF_SIZE];
    vsnprintf(buf, sizeof(buf), fmt, args);

    switch (level) {
        case ESP_LOG_ERROR: ESP_LOGE(logger->tag, "%s", buf); break;
        case ESP_LOG_WARN:  ESP_LOGW(logger->tag, "%s", buf); break;
        default:            ESP_LOGI(logger->tag, "%s", buf); break;
    }
}

void logInfo(const Logger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logV(logger, ESP_LOG_INFO, fmt, args);
    va_end(args);
}
void logWarning(const Logger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logV(logger, ESP_LOG_WARN, fmt, args);
    va_end(args);
}
void logError(const Logger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logV(logger, ESP_LOG_ERROR, fmt, args);
    va_end(args);
}