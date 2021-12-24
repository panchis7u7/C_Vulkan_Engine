#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

// TODO: Temporary.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d\n", expression, message, file, line);
}

bool8 initialize_logging() {
    //TODO: Create log file.
    return TRUE;
}

void shutdown_logging() {
    //TODO: Cleanup logging/write queued entries.
}

void log_output(Log_Level level, const char* message, ...) {
    const char *level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    bool8 error = level < LOG_LEVEL_WARN;

    const i32 msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));

    __builtin_va_list args_ptr;
    va_start(args_ptr, message);
    vsnprintf(out_message, msg_length, message, args_ptr);
    va_end(args_ptr);

    char out_message_2[msg_length];
    if(snprintf(out_message_2, msg_length, "%s%s\n", level_strings[level], out_message) <= 0)
        snprintf(out_message_2, msg_length, "%s%s", level_strings[LOG_LEVEL_ERROR], "Mensaje excedio limites del buffer!\n");

    // Platform specific output.
    if(error) {
        platform_console_write_error(out_message_2, level);
    } else {
        platform_console_write(out_message_2, level);
    }
}