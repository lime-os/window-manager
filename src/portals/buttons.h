#pragma once
#include "../all.h"

#define BUTTON_SIZE 15
#define BUTTON_PADDING 5

typedef enum {
    BUTTON_CLOSE,
    BUTTON_ARRANGE
} ButtonType;

/**
 * Draws all portal buttons (E.g. close, arrange).
 * 
 * @param portal The portal to draw the buttons for.
 */
void draw_buttons(Portal *portal);
