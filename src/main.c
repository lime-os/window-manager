#include "all.h"

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
    initialize_atoms(display, root_window);
    initialize_event_loop(display, root_window);
    return 0;
}
