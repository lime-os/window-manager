#include "../all.h"

static Portal *resized_portal = NULL;
static bool is_resizing = false;

static int mouse_start_x = 0, mouse_start_y = 0;
static int portal_start_width = 0, portal_start_height = 0;

static Time last_resize_time = 0;

static void start_resizing(Portal *portal, int mouse_x, int mouse_y)
{
    is_resizing = true;
    resized_portal = portal;
    portal_start_width = portal->width;
    portal_start_height = portal->height;
    mouse_start_x = mouse_x;
    mouse_start_y = mouse_y;
}

static void update_resizing(Display *display, int mouse_x, int mouse_y, Time event_time)
{
    if (event_time - last_resize_time < RESIZE_THROTTLE_MS) return;

    int new_portal_width = max(
        MINIMUM_WINDOW_WIDTH,
        portal_start_width + (mouse_x - mouse_start_x)
    );
    int new_portal_height = max(
        MINIMUM_WINDOW_HEIGHT,
        portal_start_height + (mouse_y - mouse_start_y)
    );

    resized_portal->width = new_portal_width;
    resized_portal->height = new_portal_height;

    XResizeWindow(display, resized_portal->frame_window, new_portal_width, new_portal_height);
    XResizeWindow(display, resized_portal->client_window, new_portal_width, new_portal_height - TITLE_BAR_HEIGHT);

    last_resize_time = event_time;
}

static void stop_resizing()
{
    is_resizing = false;
}

static bool is_resize_area(Portal *portal, int mouse_x, int mouse_y)
{
    int relative_mouse_x = mouse_x - portal->x;
    int relative_mouse_y = mouse_y - portal->y;
    
    return (relative_mouse_x >= (int)portal->width - RESIZE_AREA_SIZE && 
            relative_mouse_x <= (int)portal->width &&
            relative_mouse_x >= (int)portal->height - RESIZE_AREA_SIZE && 
            relative_mouse_y <= (int)portal->height);
}

HANDLE(ButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_resizing == true) return;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;

    if(!is_resize_area(portal, _event->x_root, _event->y_root)) return;

    start_resizing(portal, _event->x_root, _event->y_root);
}

HANDLE(ButtonRelease)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_resizing == false) return;

    stop_resizing();
}

HANDLE(MotionNotify)
{
    XMotionEvent *_event = &event->xmotion;

    if (is_resizing == false) return;

    update_resizing(display, _event->x_root, _event->y_root, _event->time);
}