#include "logger.h"

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
    //bool8 error = level < 2;

    char out_mesage[32000];
    memset(out_mesage, 0, sizeof(out_mesage));

    __builtin_va_list args_ptr;
    va_start(args_ptr, message);
    vsnprintf(out_mesage, 32000, message, args_ptr);
    va_end(args_ptr);

    char out_mesage_2[32000];
    if(snprintf(out_mesage_2, 32000, "%s%s\n", level_strings[level], out_mesage) <= 0)
        snprintf(out_mesage_2, 32000, "%s%s", level_strings[LOG_LEVEL_ERROR], "Mensaje excedio limites del buffer!\n");

    //TODO: Platform specific output.
    printf("%s", out_mesage_2);
}