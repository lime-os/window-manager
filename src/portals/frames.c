#include "../all.h"

static const long frame_event_mask =
    ExposureMask |
    SubstructureNotifyMask;

static void draw_frame(Portal *portal)
{
    Display *display = portal->display;
    Window frame_window = portal->frame_window;
    unsigned int width = portal->width;
    unsigned int height = portal->height;

    // Create a Cairo surface and context.
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_xlib_surface_create(display, frame_window, DefaultVisual(display, 0), width, height);
    cr = cairo_create(surface);

    // Draw title bar.
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_rectangle(cr, 0, 0, width, TITLE_BAR_HEIGHT);
    cairo_fill(cr);

    // Draw close button.
    draw_button(cr, width, BUTTON_CLOSE);

    // Set the color for the border around the window.
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_stroke(cr);

    // Clean up.
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

Window create_frame(Portal *portal)
{
    Display *display = portal->display;
    int x = portal->x;
    int y = portal->y;
    unsigned int width = portal->width;
    unsigned int height = portal->height;

    Window frame_window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        x, y,
        width, height,
        2, 0x000000, 0xFFFFFF
    );

    XSelectInput(display, frame_window, frame_event_mask);

    return frame_window;
}

int destroy_frame(Portal *portal)
{
    if(portal == NULL || portal->frame_window == 0)
    {
        // TODO Log in file.
        printf("Attempted to destroy non-existant frame window.\n");
        return -1;
    }
    return XDestroyWindow(portal->display, portal->frame_window);
}

HANDLE(Expose)
{
    XExposeEvent *_event = &event->xexpose;

    if (_event->count > 0) return;

    Portal *portal = find_portal(_event->window);
    if (portal == NULL) return;

    if (_event->window != portal->frame_window) return;

    draw_frame(portal);
}
