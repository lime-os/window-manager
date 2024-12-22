#pragma once
#include "../all.h"

// Custom event types.
#define Prepare 128
#define Initialize 129
#define GlobalButtonPress 130
#define GlobalButtonRelease 131
#define GlobalMotionNotify 132

/**
 * Initiates an infinite loop, handling X11/XInput2 events as they come in, and
 * invoking the appropriate registered event handlers.
 * 
 * @param display The X11 display.
 * @param root_window The X11 root window.
 */
void initialize_event_loop(Display *display, Window root_window);
