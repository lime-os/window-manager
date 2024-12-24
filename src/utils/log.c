#include "../all.h"

void log_message(const char* severity, const char* file, int line, const char* message, ...)
{
    // Get current time and format it as HH:MM:SS.
    char time_string[20];
    time_t time_now = time(NULL);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", localtime(&time_now));

    // Format the provided message with the variable arguments.
    va_list vargs;
    va_start(vargs, message);
    char formatted_message[1024];
    vsnprintf(formatted_message, sizeof(formatted_message), message, vargs);
    va_end(vargs);

    printf("[%s][%s:%d] %s: %s\n", time_string, file, line, severity, formatted_message);
}
