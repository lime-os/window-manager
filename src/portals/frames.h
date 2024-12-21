#pragma once
#include "../all.h"

#define TITLE_BAR_HEIGHT 15

/**
 * Creates a frame window and its Cairo context.
 * 
 * @param portal The portal to create the frame window for. It is assumed that
 * the portal struct is already populated with the necessary information
 * (Such as `x`, `y`, `width` and `height` etc.) to create the frame.
 * @param out_window The output parameter to store the created frame window.
 * @param out_cr The output parameter to store the Cairo context for the frame.
 */
void create_frame(Portal *portal, Window *out_window, cairo_t **out_cr);

/**
 * Destroys a frame window and performs any neccessary cleanups.
 * 
 * @param portal The portal containing the frame window.
 * @return 0 if successful, non-zero integer otherwise.
 */
int destroy_frame(Portal *portal);
