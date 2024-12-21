#include "../all.h"

static void calculate_button_position(Portal *portal, ButtonType type, int *out_x, int *out_y)
{
    // Calculate starting position.
    int x = portal->width - BUTTON_PADDING - BUTTON_SIZE;
    int y = (TITLE_BAR_HEIGHT - BUTTON_SIZE) / 2;
    
    // Calculate the position based on the button type.
    if(type == BUTTON_CLOSE)
    {
        *out_x = x;
        *out_y = y;
    }
    if(type == BUTTON_ARRANGE)
    {
        *out_x = x - BUTTON_SIZE - BUTTON_PADDING;
        *out_y = y;
    }
}

static bool is_button_area(Portal *portal, ButtonType type, int mouse_rel_x, int mouse_rel_y) {
    int button_x, button_y;
    calculate_button_position(portal, type, &button_x, &button_y);

    return (mouse_rel_x >= button_x && 
            mouse_rel_x <= button_x + BUTTON_SIZE &&
            mouse_rel_y >= button_y && 
            mouse_rel_y <= button_y + BUTTON_SIZE);
}

static void draw_button(Portal *portal, ButtonType type)
{
    cairo_t *cr = portal->frame_cr;

    int button_x, button_y;
    calculate_button_position(portal, type, &button_x, &button_y);

    // Define the button stroke style.
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr, 2);

    // Define the button path based on the button type.
    if (type == BUTTON_CLOSE)
    {
        cairo_move_to(cr,
            button_x + BUTTON_PADDING,
            button_y + BUTTON_PADDING);
        cairo_line_to(cr,
            button_x + BUTTON_SIZE - BUTTON_PADDING, 
            button_y + BUTTON_SIZE - BUTTON_PADDING);
        cairo_move_to(cr,
            button_x + BUTTON_SIZE - BUTTON_PADDING, 
            button_y + BUTTON_PADDING);
        cairo_line_to(cr,
            button_x + BUTTON_PADDING, 
            button_y + BUTTON_SIZE - BUTTON_PADDING);
    }
    if (type == BUTTON_ARRANGE)
    {
        cairo_rectangle(cr,
            button_x + BUTTON_PADDING, 
            button_y + BUTTON_PADDING,
            (BUTTON_SIZE - (2 * BUTTON_PADDING)) * 1.2,
            BUTTON_SIZE - (2 * BUTTON_PADDING));
    }

    // Render the defined path.
    cairo_stroke(cr);
}

void draw_buttons(Portal *portal)
{
    draw_button(portal, BUTTON_CLOSE);
    // draw_button(portal, BUTTON_ARRANGE);
}

HANDLE(GlobalButtonPress)
{
    XButtonEvent *_event = &event->xbutton;

    if (_event->button != Button1) return;

    Portal *portal = find_portal(_event->window);
    if(portal == NULL) return;

    if(is_portal_frame_area(portal, _event->x, _event->y) == false) return;

    if(is_button_area(portal, BUTTON_CLOSE, _event->x, _event->y))
    {
        destroy_portal(portal);
    }
    if(is_button_area(portal, BUTTON_ARRANGE, _event->x, _event->y))
    {
        // TODO: Implement the arrange button.
    }
}
