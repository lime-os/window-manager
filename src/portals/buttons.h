#pragma once
#include "../all.h"

#define BUTTON_SIZE 15
#define BUTTON_PADDING 5

typedef enum {
    BUTTON_CLOSE,
    BUTTON_MAXIMIZE,
    BUTTON_MINIMIZE
} ButtonType;

void draw_button(cairo_t *cr, unsigned int frame_width, ButtonType type);
