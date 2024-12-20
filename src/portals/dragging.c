#include "../all.h"

static Portal *dragged_portal = NULL;
static bool is_dragging = false;

static int mouse_start_root_x = 0, mouse_start_root_y = 0;
static int portal_start_x = 0, portal_start_y = 0;

static Time last_drag_time = 0;

static void start_dragging(Portal *portal, int mouse_root_x, int mouse_root_y)
{
    is_dragging = true;
    dragged_portal = portal;
    portal_start_x = portal->x;
    portal_start_y = portal->y;
    mouse_start_root_x = mouse_root_x;
    mouse_start_root_y = mouse_root_y;
}

static void update_dragging(Display *display, int mouse_root_x, int mouse_root_y, Time event_time)
{
    if (event_time - last_drag_time < RESIZE_THROTTLE_MS) return;

    int new_portal_x = portal_start_x + (mouse_root_x - mouse_start_root_x);
    int new_portal_y = portal_start_y + (mouse_root_y - mouse_start_root_y);

    dragged_portal->x = new_portal_x;
    dragged_portal->y = new_portal_y;

    XMoveWindow(
        display,
        dragged_portal->frame_window,
        new_portal_x,
        new_portal_y
    );

    last_drag_time = event_time;
}

static void stop_dragging()
{
    is_dragging = false;
}

HANDLE(GlobalButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_dragging == true) return;
    if (is_frame_area(_event->x, _event->y) == false) return;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;

    start_dragging(portal, _event->x_root, _event->y_root);
}

HANDLE(GlobalButtonRelease)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_dragging == false) return;

    stop_dragging();
}

HANDLE(GlobalMotionNotify)
{
    XMotionEvent *_event = &event->xmotion;

    if (is_dragging == false) return;

    update_dragging(display, _event->x_root, _event->y_root, _event->time);
}
