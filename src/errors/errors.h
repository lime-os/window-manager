#pragma once
#include "../all.h"

typedef enum {
    ERROR_IGNORE,   // Silently ignore.
    ERROR_LOG,      // Log and ignore.
    ERROR_FATAL     // Log and exit.
} ErrorSeverity;

/**
 * X11 error handler callback function, intended to be passed to
 * `XSetErrorHandler()`.
 * 
 * Determines severity of errors and takes appropriate action (E.g silently 
 * ignore, log and ignore, log and exit).
 *
 * @param display The X11 display.
 * @param error The X11 error event structure.
 * @return Always returns 0 to indicate to X11 that the error was handled,
 * and doesn't need to be propagated further.
 */
int error_handler(Display *display, XErrorEvent *error);
