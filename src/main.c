#include "all.h"

int main()
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        LOG_ERROR("Failed to open X11 display.");
        exit(EXIT_FAILURE);
    }

    XSetErrorHandler(error_handler);
    
    Window root_window = DefaultRootWindow(display);
    setup_ewmh_identification_chain(display, root_window);
    setup_ewmh_supported_list(display, root_window);
    initialize_event_loop(display, root_window);
    return 0;
}
