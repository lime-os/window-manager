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
 */
void create_portal(Display *display, Window client_window);

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
 * Checks if the mouse position is within the frame area of a portal.
 * 
 * @param rel_x The x coordinate, relative to the portal.
 * @param rel_y The y coordinate, relative to the portal.
 * @return True if the mouse is within the frame area, false otherwise.
 */
bool is_frame_area(int rel_x, int rel_y);

/**
 * Checks if the mouse position is within the client area of a portal.
 * 
 * @param rel_x The x coordinate, relative to the portal.
 * @param rel_y The y coordinate, relative to the portal.
 * @return True if the mouse is within the client area, false otherwise.
 */
bool is_client_area(int rel_x, int rel_y);

/**
 * Checks if the provided window is a frame window that belongs to a portal.
 * 
 * @param window The window to check.
 * @return True if the window is a frame window, false otherwise.
 */
bool is_frame_window(Window window);

/**
 * Checks if the provided window is a client window that belongs to a portal.
 * 
 * @param window The window to check.
 * @return True if the window is a client window, false otherwise.
 */
bool is_client_window(Window window);
