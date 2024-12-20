#pragma once
#include "../all.h"

#define TITLE_BAR_HEIGHT 15

/**
 * Creates a frame window.
 * 
 * @param portal The portal to create the frame window for. It is assumed that
 * the portal struct is already populated with the necessary information
 * (Such as `x`, `y`, `width` and `height` etc.) to create the frame window.
 * @return The frame window.
 */
Window create_frame(Portal *portal);

/**
 * Destroys a frame window.
 * 
 * @param portal The portal containing the frame window.
 * @return 0 if successful, non-zero integer otherwise.
 */
int destroy_frame(Portal *portal);
