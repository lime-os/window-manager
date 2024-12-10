#include "../all.h"

Portal *portals = NULL;
int portals_count = 0;

static void register_portal(Window frame_window, Window client_window)
{
    Portal *buffer = realloc(portals, (portals_count + 1) * sizeof(Portal));
    if (buffer == NULL)
    {
        // TODO Log error in file.
        perror("Failed to allocate memory while registering to portal registry\n");
        exit(EXIT_FAILURE);
    }

    portals = buffer;
    portals[portals_count].frame_window = frame_window;
    portals[portals_count].client_window = client_window;
    portals_count++;
}

static void unregister_portal(Portal *portal)
{
    int index = -1;
    for (int i = 0; i < portals_count; i++) {
        if (&portals[i] == portal) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }

    // Shift all elements after the found index to the left.
    for (int i = index; i < portals_count - 1; i++)
    {
        portals[i] = portals[i + 1];
    }

    portals_count--;

    // Resize the array to the new size.
    Portal *buffer = realloc(portals, (portals_count + 1) * sizeof(Portal));
    if (buffer == NULL)
    {
        // TODO Log error in file.
        perror("Failed to allocate memory while unregistering from portal registry\n");
        exit(EXIT_FAILURE);
    }

    portals = buffer;
}

void create_portal(Display *display, Window root_window, Window client_window)
{
    XWindowAttributes attr;
    XGetWindowAttributes(display, client_window, &attr);

    Window frame_window = create_frame(
        display, root_window,
        attr.x, attr.y,
        attr.width, attr.height
    );

    XAddToSaveSet(display, client_window);
    XReparentWindow(display, client_window, frame_window, 0, 15);
    XMapWindow(display, frame_window);
    XMapWindow(display, client_window);

    register_portal(frame_window, client_window);
}

void destroy_portal(Display *display, Portal *portal)
{
    XDestroyWindow(display, portal->frame_window);
    unregister_portal(portal);
}

Portal *find_portal_by_client_window(Window client_window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if (portals[i].client_window == client_window)
        {
            return &portals[i];
        }
    }
    return NULL;
}

Portal *find_portal_by_frame_window(Window frame_window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if (portals[i].frame_window == frame_window)
        {
            return &portals[i];
        }
    }
    return NULL;
}

bool is_frame_window(Window window)
{
    return find_portal_by_frame_window(window) != NULL;
}

bool is_client_window(Window window)
{
    return find_portal_by_client_window(window) != NULL;
}

Window *find_frame_window(Window client_window)
{
    Portal *portal = find_portal_by_client_window(client_window);
    return portal ? &portal->frame_window : NULL;
}

Window *find_client_window(Window frame_window)
{
    Portal *portal = find_portal_by_frame_window(frame_window);
    return portal ? &portal->client_window : NULL;
}

HANDLE(MapRequest)
{
    XMapRequestEvent *_event = &event->xmaprequest;
    Window client_window = _event->window;

    create_portal(display, root_window, client_window);
}

HANDLE(DestroyNotify)
{
    XDestroyWindowEvent *_event = &event->xdestroywindow;
    Window client_window = _event->window;

    Portal *portal = find_portal_by_client_window(client_window);
    if (portal != NULL)
    {
        destroy_portal(display, portal);
    }
}
