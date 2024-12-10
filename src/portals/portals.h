#pragma once
#include "../all.h"

typedef struct {
    Window frame_window;
    Window client_window;
} Portal;

void create_portal(Display *display, Window root_window, Window client_window);
void destroy_portal(Display *display, Portal *portal);

bool is_frame_window(Window window);
bool is_client_window(Window window);
Window *find_frame_window(Window client_window);
Window *find_client_window(Window frame_window);
Portal *find_portal_by_client_window(Window client_window);
Portal *find_portal_by_frame_window(Window frame_window);
