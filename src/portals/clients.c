#include "../all.h"

static const long client_event_mask =
    PropertyChangeMask;

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

    // Considering a MapRequest event can only be sent by clients, we can
    // safely assume that the window is a client window.
    Window client_window = _event->window;

    XSelectInput(display, client_window, client_event_mask);

    add_to_ewmh_client_list(display, root_window, client_window);
}

HANDLE(DestroyNotify)
{
    XDestroyWindowEvent *_event = &event->xdestroywindow;

    // Considering a DestroyNotify event can only be sent by clients, we can
    // safely assume that the window is a client window.
    Window client_window = _event->window;

    remove_from_ewmh_client_list(display, root_window, client_window);
}
