#include "../all.h"

#define TITLE_BAR_HEIGHT 15

#define BUTTON_SIZE 15
#define BUTTON_PADDING 5

typedef enum {
    BUTTON_CLOSE,
    BUTTON_MAXIMIZE,
    BUTTON_MINIMIZE
} ButtonType;

Portal *portals = NULL;
int portals_count = 0;

Window dragged_window = 0;
bool is_dragging = false;
int drag_start_x = 0, drag_start_y = 0;
int window_start_x = 0, window_start_y = 0;

static const long frame_event_mask =
    ExposureMask |
    ButtonPressMask |
    ButtonReleaseMask |
    PointerMotionMask |
    SubstructureNotifyMask;

static void configure_window(Display *display, XConfigureRequestEvent *configure_request)
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

static void focus_window(Display *display, Window window)
{
    XRaiseWindow(display, window);
}

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

static void register_portal(Window frame_window, Window client_window)
{
    Portal *buffer = realloc(portals, (portals_count + 1) * sizeof(Portal));
    if (buffer == NULL)
    {
        // TODO Log error.
        perror("Failed to allocate memory while registering to portal registry\n");
        exit(EXIT_FAILURE);
    }

    portals = buffer;
    portals[portals_count].frame_window = frame_window;
    portals[portals_count].client_window = client_window;
    portals_count++;
}

static bool is_frame_window(Window window)
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

static Window *find_frame_window(Window client_window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if(portals[i].client_window == client_window)
        {
            return &portals[i].frame_window;
        }
    }

    return NULL;
}

static Window *find_client_window(Window frame_window)
{
    for (int i = 0; i < portals_count; i++)
    {
        if(portals[i].frame_window == frame_window)
        {
            return &portals[i].client_window;
        }
    }

    return NULL;
}

static void unregister_portal(Window frame_window)
{
    int wasFound = false;

    // Find the index of the frame to be removed.
    for (int i = 0; i < portals_count; i++)
    {
        if (portals[i].frame_window == frame_window)
        {
            wasFound = true;
            // Shift all elements after the found index to the left.
            for (int j = i; j < portals_count - 1; j++)
            {
                portals[j] = portals[j + 1];
            }
            break;
        }
    }

    if (wasFound)
    {
        portals_count--;

        // Resize the array to the new size.
        Portal *buffer = realloc(portals, portals_count * sizeof(Portal));
        if (buffer == NULL && portals_count > 0)
        {
            // TODO Log error.
            perror("Failed to allocate memory while unregistering from portal registry\n");
            exit(EXIT_FAILURE);
        }
        portals = buffer;
    }
}

static Vector2 calculate_button_position(int frame_window_width, ButtonType type) {
    Vector2 pos;
    
    // Calculate starting position.
    pos.x = frame_window_width - BUTTON_PADDING - BUTTON_SIZE;
    pos.y = (TITLE_BAR_HEIGHT - BUTTON_SIZE) / 2;
    
    // Calculate the position based on the button type.
    switch (type) {
        case BUTTON_CLOSE:
            pos.x = pos.x;
            break;
        case BUTTON_MAXIMIZE:
            pos.x -= (BUTTON_SIZE + BUTTON_PADDING);
            break;
        case BUTTON_MINIMIZE:
            pos.x -= 2 * (BUTTON_SIZE + BUTTON_PADDING);
            break;
        default:
            break;
    }
    
    return pos;
}

static bool is_button_hit(int x, int y, int frame_window_width, ButtonType type) {
    Vector2 button_pos = calculate_button_position(frame_window_width, type);
    return (x >= button_pos.x && 
            x <= button_pos.x + BUTTON_SIZE &&
            y >= button_pos.y && 
            y <= button_pos.y + BUTTON_SIZE);
}

static void draw_button(cairo_t *cr, int x, int y, ButtonType type)
{
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr, 2);

    switch (type)
    {
        case BUTTON_CLOSE:
            cairo_move_to(cr,
                x + BUTTON_PADDING,
                y + BUTTON_PADDING);
            cairo_line_to(cr,
                x + BUTTON_SIZE - BUTTON_PADDING, 
                y + BUTTON_SIZE - BUTTON_PADDING);
            cairo_move_to(cr,
                x + BUTTON_SIZE - BUTTON_PADDING, 
                y + BUTTON_PADDING);
            cairo_line_to(cr,
                x + BUTTON_PADDING, 
                y + BUTTON_SIZE - BUTTON_PADDING);
            break;
        case BUTTON_MAXIMIZE:
            cairo_rectangle(cr,
                x + BUTTON_PADDING, 
                y + BUTTON_PADDING,
                BUTTON_SIZE - (2 * BUTTON_PADDING),
                BUTTON_SIZE - (2 * BUTTON_PADDING));
            break;
        case BUTTON_MINIMIZE:
            cairo_rectangle(cr,
                x + BUTTON_PADDING,
                y + BUTTON_SIZE - (2 * BUTTON_PADDING),
                BUTTON_SIZE - (2 * BUTTON_PADDING),
                BUTTON_PADDING);
            break;
        default:
            break;
    }

    cairo_stroke(cr);
}

static void draw_frame(Display *display, Window frame_window, int width, int height)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_xlib_surface_create(display, frame_window, DefaultVisual(display, 0), width, height);
    cr = cairo_create(surface);

    // Draw title bar.
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_rectangle(cr, 0, 0, width, 15);
    cairo_fill(cr);

    // Draw close button.
    Vector2 button_pos = calculate_button_position(width, BUTTON_CLOSE);
    draw_button(cr, button_pos.x, button_pos.y, BUTTON_CLOSE);

    // Set the color for the border around the window.
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_stroke(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

static void redraw_frame(Display *display, Window frame_window)
{
    XWindowAttributes attr;
    XGetWindowAttributes(display, frame_window, &attr);
    draw_frame(display, frame_window, attr.width, attr.height);
}

static void create_portal(Display *display, Window root_window, Window client_window)
{
    XWindowAttributes attr;
    XGetWindowAttributes(display, client_window, &attr);

    Window frame_window = XCreateSimpleWindow(
        display,
        root_window,
        attr.x, attr.y,
        attr.width, attr.height,
        2, 0x000000, 0xFFFFFF);

    register_portal(frame_window, client_window);

    XSelectInput(display, frame_window, frame_event_mask);
    XAddToSaveSet(display, client_window);
    XReparentWindow(display, client_window, frame_window, 0, 15);
    XMapWindow(display, frame_window);
    XMapWindow(display, client_window);

    draw_frame(display, frame_window, attr.width, attr.height + 15);
}

static void destroy_portal(Display *display, Window frame_window)
{
    XDestroyWindow(display, frame_window);
    unregister_portal(frame_window);
}

HANDLE(ConfigureRequest)
{
    configure_window(display, &event->xconfigurerequest);
}

HANDLE(MapRequest)
{
    create_portal(display, root_window, event->xmaprequest.window);
    add_to_client_list_atom(display, root_window, event->xmaprequest.window);
}

HANDLE(DestroyNotify)
{
    Window *frame_window = find_frame_window(event->xdestroywindow.window);
    if (frame_window != NULL)
    {
        destroy_portal(display, *frame_window);
    }

    remove_from_client_list_atom(display, root_window, event->xdestroywindow.window);
}

HANDLE(ButtonPress)
{
    if (event->xbutton.button == Button1)
    {
        XButtonEvent button_event = event->xbutton;

        // Handle `close` button click.
        XWindowAttributes attr;
        XGetWindowAttributes(display, button_event.window, &attr);
        int window_width = attr.width;
        if(is_button_hit(button_event.x, button_event.y, window_width, BUTTON_CLOSE))
        {
            Window *client_window = find_client_window(button_event.window);
            if(client_window != NULL)
            {
                XEvent close_event;
                close_event.xclient.type = ClientMessage;
                close_event.xclient.window = *client_window;
                close_event.xclient.message_type = wm_protocols;
                close_event.xclient.format = 32;
                close_event.xclient.data.l[0] = wm_delete_window;
                close_event.xclient.data.l[1] = CurrentTime;
                XSendEvent(display, *client_window, False, NoEventMask, &close_event);
                return;
            }
        }

        focus_window(display, button_event.window);

        if (is_dragging == false)
        {
            start_dragging(display, button_event.window, button_event.x_root, button_event.y_root);
        }
    }
}

HANDLE(ButtonRelease)
{
    if (event->xbutton.button == Button1 && is_dragging == true)
    {
        stop_dragging();
    }
}

HANDLE(MotionNotify)
{
    if (is_dragging == true)
    {
        XMotionEvent motion_event = event->xmotion;
        update_dragging(display, motion_event.x_root, motion_event.y_root);
    }
}

HANDLE(ClientMessage)
{
    if (event->xclient.message_type == wm_protocols &&
        (Atom)event->xclient.data.l[0] == wm_delete_window)
    {
        XDestroyWindow(display, event->xclient.window);
    }
}

HANDLE(Expose)
{
    if (event->xexpose.count == 0)
    {
        Window exposed_window = event->xexpose.window;
        if (is_frame_window(exposed_window))
        {
            redraw_frame(display, exposed_window);
        }
    }
}