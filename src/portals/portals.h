#pragma once
#include "../all.h"

typedef struct {
    Display *display;
    int x, y;
    unsigned int width, height;
    Window frame_window;
    Window client_window;
} Portal;

/**
 * Creates a portal and registers it to the portal registry.
 * 
 * @param display The display where the portal will be located.
 * @param client_window The client window to create the portal from.
 * @return The created portal, or NULL upon failure.
 */
Portal *create_portal(Display *display, Window client_window);

/**
 * Destroys a portal and unregisters it from the portal registry.
 * 
 * @param portal The portal to destroy.
 */
void destroy_portal(Portal *portal);

/**
 * Finds a portal in the portal registry using the `window` provided.
 * 
 * @param window A client or frame window.
 * @return The found portal or NULL if not found.
 */
Portal *find_portal(Window window);

/**
 * Checks if the provided coordinates are within the frame area of the portal.
 * 
 * @param portal The portal to check the frame area for.
 * @param rel_x The x coordinate, relative to the portal.
 * @param rel_y The y coordinate, relative to the portal.
 * @return True if the mouse is within the frame area, false otherwise.
 */
bool is_portal_frame_area(Portal *portal, int rel_x, int rel_y);

/**
 * Checks if the provided coordinates are within the client area of the portal.
 * 
 * @param portal The portal to check the client area for.
 * @param rel_x The x coordinate, relative to the portal.
 * @param rel_y The y coordinate, relative to the portal.
 * @return True if the mouse is within the client area, false otherwise.
 */
bool is_portal_client_area(Portal *portal, int rel_x, int rel_y);
