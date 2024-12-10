#include "../all.h"

static void handle_configure_request(Display *display, XConfigureRequestEvent *configure_request)
{
    XWindowChanges changes;
    changes.x = configure_request->x;
    changes.y = configure_request->y;
    changes.width = configure_request->width;
    changes.height = configure_request->height;
    changes.border_width = configure_request->border_width;
    changes.sibling = configure_request->above;
    changes.stack_mode = configure_request->detail;

    XConfigureWindow(
        display,
        configure_request->window,
        configure_request->value_mask,
        &changes);
}

HANDLE(ConfigureRequest)
{
    XConfigureRequestEvent *_event = &event->xconfigurerequest;
    
    handle_configure_request(display, _event);
}

HANDLE(MapRequest)
{
    XMapRequestEvent *_event = &event->xmaprequest;
    Window client_window = _event->window;

    add_to_client_list_atom(display, root_window, client_window);
}

HANDLE(DestroyNotify)
{
    XDestroyWindowEvent *_event = &event->xdestroywindow;
    Window client_window = _event->window;

    remove_from_client_list_atom(display, root_window, client_window);
}
