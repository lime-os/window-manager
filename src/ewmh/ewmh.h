#pragma once
#include "../all.h"

/**
 * Establishes the identification chain required by the Extended Window Manager 
 * Hints (EWMH) specification. This allows other applications to: detect 
 * presence of our window manager, verify its identity and query its name.
 * 
 * This is a core requirement of the EWMH specification. Without it, some
 * applications may simply refuse to work with our window manager.
 *
 * @param display X11 display connection.
 * @param root_window Root window of the display.
 *
 * @note https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.12
 */
void setup_ewmh_identification_chain(Display *display, Window root_window);

/**
 * Advertises the supported EWMH features on the root window. This allows other 
 * applications to query the window manager for its capabilities.
 * 
 * This is a core requirement of the EWMH specification. Without it, some
 * applications may simply refuse to work with our window manager.
 *
 * @param display X11 display connection.
 * @param root_window Root window of the display.
 *
 * @note https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.3
 */
void setup_ewmh_supported_list(Display *display, Window root_window);
