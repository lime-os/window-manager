#include "../all.h"

static Vector2 calculate_button_position(unsigned int frame_width, ButtonType type) {
    Vector2 pos;
    
    // Calculate starting position.
    pos.x = frame_width - BUTTON_PADDING - BUTTON_SIZE;
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

static bool is_button_hit(int mouse_rel_x, int mouse_rel_y, unsigned int frame_width, ButtonType type) {
    Vector2 button_pos = calculate_button_position(frame_width, type);
    return (mouse_rel_x >= button_pos.x && 
            mouse_rel_x <= button_pos.x + BUTTON_SIZE &&
            mouse_rel_y >= button_pos.y && 
            mouse_rel_y <= button_pos.y + BUTTON_SIZE);
}

static void handle_close_button_click(Display *display, Window frame_window) {
    Portal *portal = find_portal(frame_window);
    if(portal != NULL)
    {
        destroy_portal(display, portal);
    }
}

void draw_button(cairo_t *cr, unsigned int frame_width, ButtonType type)
{
    Vector2 pos = calculate_button_position(frame_width, type);
    int x = pos.x;
    int y = pos.y;

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

HANDLE(GlobalButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;
    if(is_frame_area(_event->x, _event->y) == false) return;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;

    XWindowAttributes attr;
    XGetWindowAttributes(display, portal->frame_window, &attr);
    unsigned int frame_width = attr.width;

    if(is_button_hit(_event->x, _event->y, frame_width, BUTTON_CLOSE))
    {
        handle_close_button_click(display, portal->frame_window);
    }
}
