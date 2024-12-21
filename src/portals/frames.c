#include "../all.h"

static const long frame_event_mask =
    ExposureMask |
    SubstructureNotifyMask;

static void draw_frame(Portal *portal)
{
    cairo_t *cr = portal->frame_cr;
    unsigned int width = portal->width;
    unsigned int height = portal->height;

    // Resize the Cairo surface.
    cairo_xlib_surface_set_size(cairo_get_target(cr), width, height);

    // Draw title bar.
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_rectangle(cr, 0, 0, width, TITLE_BAR_HEIGHT);
    cairo_fill(cr);

    // Draw buttons (E.g. close, arrange).
    draw_buttons(portal);

    // Draw the border around the window.
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_stroke(cr);
}

void create_frame(Portal *portal, Window *out_window, cairo_t **out_cr)
{
    Display *display = portal->display;
    int x = portal->x;
    int y = portal->y;
    unsigned int width = portal->width;
    unsigned int height = portal->height;

    // Create the frame window.
    Window frame_window = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        x, y,
        width, height,
        2, 0x000000, 0xFFFFFF
    );

    // Listen to frame window events.
    XSelectInput(display, frame_window, frame_event_mask);

    // Create the Cairo context for the frame window.
    cairo_surface_t *surface = cairo_xlib_surface_create(
        display,
        frame_window,
        DefaultVisual(display, 0),
        width, height
    );
    cairo_t *cr = cairo_create(surface);

    *out_window = frame_window;
    *out_cr = cr;
}

int destroy_frame(Portal *portal)
{
    if(portal == NULL || portal->frame_window == 0) return -1;

    cairo_surface_t *surface = cairo_get_target(portal->frame_cr);
    cairo_destroy(portal->frame_cr);
    cairo_surface_destroy(surface);

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
