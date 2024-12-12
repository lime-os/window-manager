#include "../all.h"

Window dragged_window = 0;
bool is_dragging = false;
int drag_start_x = 0, drag_start_y = 0;
int window_start_x = 0, window_start_y = 0;

static void start_dragging(Display *display, Window window, int mouse_x, int mouse_y)
{
    XWindowAttributes attr;
    if (!XGetWindowAttributes(display, window, &attr))
    {
        return;
    }

    is_dragging = true;
    dragged_window = window;
    drag_start_x = mouse_x;
    drag_start_y = mouse_y;
    window_start_x = attr.x;
    window_start_y = attr.y;
}

static void update_dragging(Display *display, int mouse_x, int mouse_y)
{
    int dx = mouse_x - drag_start_x;
    int dy = mouse_y - drag_start_y;

    XMoveWindow(display, dragged_window, window_start_x + dx, window_start_y + dy);
}

static void stop_dragging()
{
    is_dragging = false;
}

HANDLE(ButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_dragging == true) return;

    Window frame_window = find_frame_window(_event->window, _event->subwindow);
    if(frame_window == 0) return;

    start_dragging(display, frame_window, _event->x_root, _event->y_root);
}

HANDLE(ButtonRelease)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_dragging == false) return;

    Window frame_window = find_frame_window(_event->window, _event->subwindow);
    if(frame_window == 0) return;

    stop_dragging();
}

HANDLE(MotionNotify)
{
    XMotionEvent *_event = &event->xmotion;

    if (is_dragging == false) return;

    update_dragging(display, _event->x_root, _event->y_root);
}
