#include "../all.h"

static void focus_window(Display *display, Window window) {
    if (is_client_window(window)) {
        Window *frame_window = find_frame_window(window);
        if(frame_window != NULL)
        {
            XRaiseWindow(display, *frame_window);
        }
        return;
    }

    if (is_frame_window(window)) {
        XRaiseWindow(display, window);
    }
}

HANDLE(ButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button == Button1) {
        focus_window(display, _event->window);
    }
}
