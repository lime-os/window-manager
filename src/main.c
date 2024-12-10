#include "all.h"

static const long root_event_mask =
    SubstructureRedirectMask |
    SubstructureNotifyMask;

int main()
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        // TODO Store in a log file.
        fprintf(stderr, "Failed to open display\n");
        exit(EXIT_FAILURE);
    }

    XSetErrorHandler(error_handler);

    Window root_window = DefaultRootWindow(display);
    XSelectInput(display, root_window, root_event_mask);
    
    initialize_atoms(display, root_window);
    initialize_event_loop(display, root_window);
    return 0;
}
