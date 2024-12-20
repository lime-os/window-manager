#include "../all.h"

static void focus_portal(Portal *portal) {
    XRaiseWindow(portal->display, portal->frame_window);
}

HANDLE(GlobalButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;
    
    focus_portal(portal);
}
