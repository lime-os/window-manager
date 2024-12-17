#pragma once
#include "../all.h"

typedef struct {
    int x, y;
    unsigned int width, height;
    Window frame_window;
    Window client_window;
} Portal;

void create_portal(Display *display, Window root_window, Window client_window);
void destroy_portal(Display *display, Portal *portal);
Portal *find_portal(Window window);

bool is_frame_area(int x, int y);
bool is_client_area(int x, int y);
bool is_frame_window(Window window);
bool is_client_window(Window window);
