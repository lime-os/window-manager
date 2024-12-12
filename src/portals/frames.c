#include "../all.h"

static const long frame_event_mask =
    ExposureMask |
    SubstructureNotifyMask;

static const long frame_grab_mask =
    PointerMotionMask |
    ButtonPressMask |
    ButtonReleaseMask;

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
    XGrabButton(display,
        AnyButton,          // Grab all mouse buttons.
        AnyModifier,        // Grab regardless of modifier keys.
        frame_window,       // Window to grab events for.
        True,               // Allow events to propagate.
        frame_grab_mask,    // Event mask.
        GrabModeAsync,      // Don't freeze pointer movement.
        GrabModeAsync,      // Don't freeze keyboard input.
        None,               // Don't confine the cursor.
        None                // Don't change the cursor.
    );

    draw_frame(display, frame_window, width, height);

    return frame_window;
}

HANDLE(Expose)
{
    XExposeEvent *_event = &event->xexpose;

    if (_event->count != 0) return;
    
    Window frame_window = find_frame_window(_event->window, 0);
    if (frame_window == 0) return;

    XWindowAttributes attr;
    XGetWindowAttributes(display, frame_window, &attr);

    draw_frame(display, frame_window, attr.width, attr.height);
}
