#include "all.h"

static ErrorSeverity get_error_severity(int error_code) {
    switch (error_code) {
        case BadWindow:
        case BadDrawable:
        case BadPixmap:
            return ERROR_IGNORE;

        case BadMatch:
        case BadValue:
        case BadAccess:
            return ERROR_LOG;

        default:
            return ERROR_FATAL;
    }
}

// TODO Log error in file.
static void print_error(Display *display, XErrorEvent *error) {
    char error_text[1024];
    XGetErrorText(display, error->error_code, error_text, sizeof(error_text));
    fprintf(stderr, "X Error: %s\n", error_text);
}

int error_handler(Display *display, XErrorEvent *error) {
    ErrorSeverity severity = get_error_severity(error->error_code);

    if(severity == ERROR_LOG)
    {
        print_error(display, error);
    }
    if(severity == ERROR_FATAL)
    {
        print_error(display, error);
        exit(EXIT_FAILURE);
    }

    return 0;
}
