#include "../all.h"

static void set_portal_title(Portal *portal, const char *title)
{
    free(portal->title);
    portal->title = strdup(title);
}

void draw_portal_title(Portal *portal)
{
    cairo_t *cr = portal->frame_cr;
    unsigned int width = portal->width;

    // Set the font and color.
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10.0);

    // Determine the position for the title text.
    cairo_text_extents_t title_extents;
    cairo_text_extents(cr, portal->title, &title_extents);
    double title_x = (width - title_extents.width) / 2;
    double title_y = (TITLE_BAR_HEIGHT + title_extents.height) / 2;
    cairo_move_to(cr, title_x, title_y);

    // Draw the title text.
    cairo_show_text(cr, portal->title);
}

HANDLE(PropertyNotify)
{
    XPropertyEvent *_event = &event->xproperty;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;

    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom wm_name = XInternAtom(display, "WM_NAME", False);
    if (_event->atom == wm_name || _event->atom == net_wm_name)
    {
        char *title = malloc(256);
        x_get_window_name(display, portal->client_window, title, 256);
        if(title != NULL)
        {
            set_portal_title(portal, title);
            draw_frame(portal);
            free(title);
        }
    }
}
