#pragma once
#include "../all.h"

extern Atom wm_protocols;
extern Atom wm_delete_window;
extern Atom net_supporting_wm_check;
extern Atom net_wm_name;
extern Atom net_client_list;
extern Atom net_supported;

/**
 * Initializes X11 atom identifiers and configures standard window manager
 * properties according to EWMH/ICCCM specifications.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 */
void initialize_atoms(Display *display, Window root_window);

/**
 * Adds a window to the _NET_CLIENT_LIST atom. EWMH specification requires
 * every window manager to maintain a list of client windows this way so that
 * other applications can query it. Note that this is not the list that
 * we use internally to track client windows.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 * @param window The window to add to the list.
 */
void add_to_client_list_atom(Display *display, Window root_window, Window window);

/**
 * Removes a window from the _NET_CLIENT_LIST atom. EWMH specification requires
 * every window manager to maintain a list of client windows this way so that
 * other applications can query it. Note that this is not the list that
 * we use internally to track client windows.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 * @param window The window to remove from the list.
 */
void remove_from_client_list_atom(Display *display, Window root_window, Window window);
