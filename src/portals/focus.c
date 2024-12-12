#include "../all.h"

static void focus_window(Display *display, Window window, Window subwindow) {
    Window client_window = find_client_window(window, subwindow);
    if (client_window != 0) {
        Portal *portal = find_portal(client_window);
        if(portal != NULL)
        {
            XRaiseWindow(display, portal->frame_window);
        }
        return;
    }

    Window frame_window = find_frame_window(window, subwindow);
    if (frame_window != 0) {
        XRaiseWindow(display, frame_window);
        return;
    }
}

HANDLE(ButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    
    focus_window(display, _event->window, _event->subwindow);
}
