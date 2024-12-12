#pragma once
#include "../all.h"

typedef struct {
    Window frame_window;
    Window client_window;
} Portal;

void create_portal(Display *display, Window root_window, Window client_window);
void destroy_portal(Display *display, Portal *portal);

Portal *find_portal(Window window);
Window find_frame_window(Window window, Window subwindow);
Window find_client_window(Window window, Window subwindow);
