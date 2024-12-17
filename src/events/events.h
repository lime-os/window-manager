#pragma once
#include "../all.h"

void initialize_event_loop(Display *display, Window root_window);

// Custom event types.
#define Prepare 128
#define Initialize 129
#define GlobalButtonPress 130
#define GlobalButtonRelease 131
#define GlobalMotionNotify 132
