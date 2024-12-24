#pragma once
#include "../all.h"

/**
 * @brief Retrieves the name of an X11 window.
 * 
 * Attempts to retrieve the name of the window using the `_NET_WM_NAME` property
 * first. If unsuccesful, it will try to retrieve the name using the `WM_NAME`
 * property, which is an older standard, but ensures compatibility with legacy
 * applications.
 * 
 * @param display The display where the window is located.
 * @param window The window to retrieve the name for.
 * @param out_name The buffer where the name will be stored. Memory for it must
 * be allocated by the caller beforehand.
 * @param name_size The size of the `out_name` buffer.
 * 
 * @return 0 on success, non-zero integer otherwise.
 */
int x_get_window_name(Display *display, Window window, char *out_name, size_t name_size);
