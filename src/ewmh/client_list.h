#pragma once
#include "../all.h"

/**
 * Adds a window to the `_NET_CLIENT_LIST` property on the root window. EWMH 
 * specification requires every window manager to maintain a list of client 
 * windows this way so that other applications can query it.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 * @param window The window to add to the list.
 * 
 * @note This is not a list that we use internally to keep track of client 
 * windows.
 */
void add_to_ewmh_client_list(Display *display, Window root_window, Window window);

/**
 * Removes a window from the `_NET_CLIENT_LIST` property on the root window. 
 * EWMH specification requires every window manager to maintain a list of client 
 * windows this way so that other applications can query it.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 * @param window The window to remove from the list.
 * 
 * @note This is not a list that we use internally to keep track of client 
 * windows.
 */
void remove_from_ewmh_client_list(Display *display, Window root_window, Window window);
