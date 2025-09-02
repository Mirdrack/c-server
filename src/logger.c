#include "logger.h"
#include <stdarg.h>
#include <string.h>

static log_level_t current_level = LOG_INFO;

static const char *level_to_str(log_level_t level)
{
    switch (level)
    {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARN:
        return "WARN";
    case LOG_ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

void set_log_level(log_level_t level)
{
    current_level = level;
}

// Get ISO8601 timestamp
static void current_timestap(char *buffer, size_t size)
{
    time_t now = time(NULL);
    const struct tm *gm_time = gmtime(&now); // UTC
    strftime(buffer, size, "%Y-%m-%dT%H:%M:%SZ", gm_time);
}

void log_message(log_level_t level, const char *file, int line, const char *fmt, ...)
{
    if (level < current_level)
    {
        return; // Skip logs below current level
    }

    char timestamp[TIMESTAMP_LEN];
    current_timestap(timestamp, sizeof(timestamp));

    char message[LOG_MESSAGE_LEN];

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    FILE *out = (level == LOG_WARN || level == LOG_ERROR) ? stderr : stdout;

    fprintf(out,
            "{ \"timestamp\": \"%s\", \"level\": \"%s\", \"file\": \"%s\", \"line\": %d, "
            "\"message\": \"%s\" }\n",
            timestamp, level_to_str(level), file, line, message);

    fflush(out);
}
