#include "../all.h"

static Portal *resized_portal = NULL;
static bool is_resizing = false;

static int mouse_start_root_x = 0, mouse_start_root_y = 0;
static int portal_start_width = 0, portal_start_height = 0;

static Time last_resize_time = 0;

static void start_resizing(Portal *portal, int mouse_root_x, int mouse_root_y)
{
    is_resizing = true;
    resized_portal = portal;
    portal_start_width = portal->width;
    portal_start_height = portal->height;
    mouse_start_root_x = mouse_root_x;
    mouse_start_root_y = mouse_root_y;
}

static void update_resizing(int mouse_root_x, int mouse_root_y, Time event_time)
{
    if (event_time - last_resize_time < RESIZE_THROTTLE_MS) return;

    // Calculate new portal width and height.
    int new_portal_width = max(
        MINIMUM_PORTAL_WIDTH,
        portal_start_width + (mouse_root_x - mouse_start_root_x)
    );
    int new_portal_height = max(
        MINIMUM_PORTAL_HEIGHT,
        portal_start_height + (mouse_root_y - mouse_start_root_y)
    );

    // Update portal struct with new width and height.
    resized_portal->width = new_portal_width;
    resized_portal->height = new_portal_height;

    // Resize the frame and client windows.
    XResizeWindow(
        resized_portal->display,
        resized_portal->frame_window,
        new_portal_width, new_portal_height
    );
    XResizeWindow(
        resized_portal->display,
        resized_portal->client_window,
        new_portal_width, new_portal_height - TITLE_BAR_HEIGHT
    );

    last_resize_time = event_time;
}

static void stop_resizing()
{
    is_resizing = false;

    // Notify the client window of the new size.
    XConfigureEvent configure_event = {
        .type = ConfigureNotify,
        .display = resized_portal->display,
        .event = resized_portal->client_window,
        .window = resized_portal->client_window,
        .x = resized_portal->x,
        .y = resized_portal->y,
        .width = resized_portal->width,
        .height = resized_portal->height - TITLE_BAR_HEIGHT,
        .border_width = 0,
        .above = None,
        .override_redirect = False
    };
    XSendEvent(
        resized_portal->display,
        resized_portal->client_window,
        False,
        StructureNotifyMask,
        (XEvent *)&configure_event
    );
}

static bool is_resize_area(Portal *portal, int mouse_rel_x, int mouse_rel_y)
{
    return (mouse_rel_x >= (int)portal->width - RESIZE_AREA_SIZE && 
            mouse_rel_x <= (int)portal->width &&
            mouse_rel_y >= (int)portal->height - RESIZE_AREA_SIZE && 
            mouse_rel_y <= (int)portal->height);
}

HANDLE(GlobalButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_resizing == true) return;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;

    if(!is_resize_area(portal, _event->x, _event->y)) return;

    start_resizing(portal, _event->x_root, _event->y_root);
}

HANDLE(GlobalButtonRelease)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if (is_resizing == false) return;

    stop_resizing();
}

HANDLE(GlobalMotionNotify)
{
    XMotionEvent *_event = &event->xmotion;

    if (is_resizing == false) return;

    update_resizing(_event->x_root, _event->y_root, _event->time);
}
