#include "../all.h"

Portal *portals = NULL;
int portals_count = 0;

static void register_portal(Window frame_window, Window client_window, int x, int y, unsigned int width, unsigned int height)
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
    portals[portals_count].x = x;
    portals[portals_count].y = y;
    portals[portals_count].width = width;
    portals[portals_count].height = height;
    portals_count++;
}

static void unregister_portal(Portal *portal)
{
    int index = -1;
    for (int i = 0; i < portals_count; i++)
    {
        if (&portals[i] == portal)
        {
            index = i;
            break;
        }
    }
    if (index == -1) return;

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
    XReparentWindow(display, client_window, frame_window, 0, TITLE_BAR_HEIGHT);
    XMapWindow(display, frame_window);
    XMapWindow(display, client_window);

    register_portal(frame_window, client_window, attr.x, attr.y, attr.width, attr.height);
}

void destroy_portal(Display *display, Portal *portal)
{
    XDestroyWindow(display, portal->frame_window);
    unregister_portal(portal);
}

Portal *find_portal(Window window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if (portals[i].frame_window == window ||
            portals[i].client_window == window)
        {
            return &portals[i];
        }
    }
    return NULL;
}

bool is_frame_area(int mouse_rel_x, int mouse_rel_y)
{
    (void)mouse_rel_x;
    return mouse_rel_y <= TITLE_BAR_HEIGHT;
}

bool is_client_area(int mouse_rel_x, int mouse_rel_y)
{
    (void)mouse_rel_x;
    return mouse_rel_y > TITLE_BAR_HEIGHT;
}

bool is_frame_window(Window window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if (portals[i].frame_window == window)
        {
            return true;
        }
    }
    return false;
}

bool is_client_window(Window window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if (portals[i].client_window == window)
        {
            return true;
        }
    }
    return false;
}

HANDLE(MapRequest)
{
    XMapRequestEvent *_event = &event->xmaprequest;

    // Considering a MapRequest event can only be sent by clients, we can
    // safely assume that the window is a client window.
    Window client_window = _event->window;

    Portal *portal = find_portal(_event->window);
    if (portal != NULL) return;

    create_portal(display, root_window, client_window);
}

HANDLE(DestroyNotify)
{
    XDestroyWindowEvent *_event = &event->xdestroywindow;

    // Considering a DestroyNotify event can only be sent by clients, we can
    // safely assume that the window is a client window.
    Window client_window = _event->window;

    Portal *portal = find_portal(client_window);
    if (portal == NULL) return;

    destroy_portal(display, portal);
}
