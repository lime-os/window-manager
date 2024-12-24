#include "../all.h"

static Window *client_list = NULL;
static size_t client_count = 0;
static size_t client_capacity = 0;

static void update_ewmh_client_list(Display *display, Window root_window) {
    Atom net_client_list = XInternAtom(display, "_NET_CLIENT_LIST", False);
    
    XChangeProperty(
        display,
        root_window,
        net_client_list,
        XA_WINDOW,
        32,
        PropModeReplace,
        (unsigned char *)client_list,
        client_count
    );
}

void add_to_ewmh_client_list(Display *display, Window root_window, Window window) {
    if (client_count >= client_capacity) {
        size_t new_capacity = (client_capacity == 0) ? 16 : client_capacity * 2;
        Window *new_array = realloc(client_list, new_capacity * sizeof(Window));
        if (!new_array) {
            // TODO Store in a log file.
            printf("Failed to allocate memory for EWMH client list\n");
            return;
        }
        client_list = new_array;
        client_capacity = new_capacity;
    }
    
    client_list[client_count++] = window;

    update_ewmh_client_list(display, root_window);
}

void remove_from_ewmh_client_list(Display *display, Window root_window, Window window) {
    for (size_t i = 0; i < client_count; i++) {
        if (client_list[i] == window) {
            memmove(&client_list[i], &client_list[i + 1], (client_count - i - 1) * sizeof(Window));
            client_count--;

            update_ewmh_client_list(display, root_window);
            break;
        }
    }
}
