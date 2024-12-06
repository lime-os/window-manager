#include "all.h"

Atom wm_protocols;
Atom wm_delete_window;
Atom net_supporting_wm_check;
Atom net_wm_name;
Atom net_client_list;
Atom net_supported;

static Window *client_windows = NULL;
static size_t client_count = 0;
static size_t client_capacity = 0;

void initialize_atoms(Display *display, Window root_window)
{
    // Get atom ID for WM_PROTOCOLS (predefined X11 core atom).
    // Used to read which protocols each client window supports.
    // See: https://tronche.com/gui/x/icccm/sec-4.html#s-4.1.2.7
    wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);

    // Get atom ID for WM_DELETE_WINDOW (ICCCM protocol).
    // Used in ClientMessage events when a window requests graceful closure.
    // See: https://tronche.com/gui/x/icccm/sec-4.html#s-4.2.8.1
    wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);

    // Get atom ID for _NET_CLIENT_LIST - EWMH property that provides 
    // allows other applications to query a list of managed windows.
    // See: https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.4
    net_client_list = XInternAtom(display, "_NET_CLIENT_LIST", False);

    // Set up EWMH window manager identification chain:
    // 1. Create hidden check window.
    // 2. Set _NET_SUPPORTING_WM_CHECK on check window pointing to itself.
    // 3. Set _NET_SUPPORTING_WM_CHECK on root window pointing to check window.
    // See: https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.12
    net_supporting_wm_check = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
    Window check_window = XCreateSimpleWindow(display, root_window, -1, -1, 1, 1, 0, 0, 0);
    XChangeProperty(display, check_window, net_supporting_wm_check, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&check_window, 1);
    XChangeProperty(display, root_window, net_supporting_wm_check, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&check_window, 1);
    
    // Set window manager name on check window.
    // Applications can read this to identify the active window manager.
    // See: https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.12
    net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
    const char *wm_name = "Lime Window Manager";
    XChangeProperty(display, check_window, net_wm_name, utf8_string, 8, PropModeReplace, (unsigned char *)wm_name, strlen(wm_name));

    // Advertise supported EWMH features on root window.
    // Applications check this to determine available window management capabilities.
    // See: https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.3
    net_supported = XInternAtom(display, "_NET_SUPPORTED", False);
    Atom supported_atoms[] = {
        net_supporting_wm_check,
        net_wm_name,
        net_client_list
    };
    XChangeProperty(display, root_window, net_supported, XA_ATOM, 32, PropModeReplace, (unsigned char *)supported_atoms, sizeof(supported_atoms) / sizeof(Atom));
}

static void update_client_list_atom(Display *display, Window root_window) {
    XChangeProperty(
        display,
        root_window,
        net_client_list,
        XA_WINDOW,
        32,
        PropModeReplace,
        (unsigned char *)client_windows,
        client_count
    );
}

/**
 * @brief Adds a window to the _NET_CLIENT_LIST atom. EWMH specification
 * requires every window manager to maintain a list of client windows this way.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 * @param window The window to add to the list.
 */
void add_to_client_list_atom(Display *display, Window root_window, Window window) {
    if (client_count >= client_capacity) {
        size_t new_capacity = (client_capacity == 0) ? 16 : client_capacity * 2;
        Window *new_array = realloc(client_windows, new_capacity * sizeof(Window));
        if (!new_array) {
            // TODO Store in a log file.
            printf("Failed to allocate memory for EWMH client list\n");
            return;
        }
        client_windows = new_array;
        client_capacity = new_capacity;
    }
    
    client_windows[client_count++] = window;

    update_client_list_atom(display, root_window);
}

/**
 * @brief Removes a window from the _NET_CLIENT_LIST atom. EWMH specification
 * requires every window manager to maintain a list of client windows this way.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 * @param window The window to remove from the list.
 */
void remove_from_client_list_atom(Display *display, Window root_window, Window window) {
    for (size_t i = 0; i < client_count; i++) {
        if (client_windows[i] == window) {
            memmove(&client_windows[i], &client_windows[i + 1], (client_count - i - 1) * sizeof(Window));
            client_count--;

            update_client_list_atom(display, root_window);
            break;
        }
    }
}
