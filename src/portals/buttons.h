#pragma once
#include "../all.h"

#define PORTAL_BUTTON_SIZE 15
#define PORTAL_BUTTON_PADDING 5

typedef enum {
    BUTTON_CLOSE,
    BUTTON_ARRANGE
} PortalButtonType;

/**
 * Draws all portal buttons (E.g. close, arrange).
 * 
 * @param portal The portal to draw the buttons for.
 * 
 * @note Intended to be used by `draw_portal_frame()`.
 */
void draw_portal_buttons(Portal *portal);
