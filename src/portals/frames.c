#include "../all.h"

static const long frame_event_mask =
    ExposureMask |
    SubstructureNotifyMask;

static void draw_frame(Display *display, Window frame_window, unsigned int width, unsigned int height)
{
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

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

Window create_frame(Display *display, Window root_window, int x, int y, unsigned int width, unsigned int height) {
    Window frame_window = XCreateSimpleWindow(
        display, root_window,
        x, y, width, height,
        2, 0x000000, 0xFFFFFF
    );

    XSelectInput(display, frame_window, frame_event_mask);

    draw_frame(display, frame_window, width, height);

    return frame_window;
}

HANDLE(Expose)
{
    XExposeEvent *_event = &event->xexpose;

    if (_event->count != 0) return;
    if (is_frame_window(_event->window) == false) return;

    Portal *portal = find_portal(_event->window);
    if (portal == NULL) return;

    XWindowAttributes attr;
    XGetWindowAttributes(display, portal->frame_window, &attr);

    draw_frame(display, portal->frame_window, attr.width, attr.height);
}
