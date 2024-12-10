#pragma once
#include "../all.h"

typedef enum {
    ERROR_IGNORE,   // Silently ignore.
    ERROR_LOG,      // Log and ignore.
    ERROR_FATAL     // Log and exit.
} ErrorSeverity;

int error_handler(Display *display, XErrorEvent *error);
