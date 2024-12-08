#include "../all.h"

EventHandlerMap *event_handler_map = NULL;
static int event_handler_map_size = 0;

static const long event_mask =
    ExposureMask |
    KeyPressMask |
    SubstructureRedirectMask |
    SubstructureNotifyMask;

static bool isRunning = false;

static void invoke_handlers(int event_type, XEvent *event, Display *display, Window window) {
    for(int i = 0; i < event_handler_map_size; i++) {
        if(event_handler_map[i].event_type == event_type) {
            event_handler_map[i].event_handler(event, display, window);
        }
    }
}

void initialize_event_loop(Display *display, Window root_window)
{
    if (isRunning == true)
    {
        printf("Event loop has already been initialized\n");
        return;
    }

    XEvent event;
    isRunning = true;

    // Choose which events the root window should listen to.
    XSelectInput(display, root_window, event_mask);

    // Invoke all preparation and initialization handler functions.
    invoke_handlers(Prepare, NULL, display, root_window);
    invoke_handlers(Initialize, NULL, display, root_window);

    // Send an initial Expose event.
    XWindowAttributes root_window_attr;
    XGetWindowAttributes(display, root_window, &root_window_attr);
    event.type = Expose;
    event.xexpose.window = root_window;
    event.xexpose.display = display;
    event.xexpose.x = 0;
    event.xexpose.y = 0;
    event.xexpose.width = root_window_attr.width;
    event.xexpose.height = root_window_attr.height;
    event.xexpose.count = 0;
    XSendEvent(display, root_window, False, ExposureMask, &event);
    XFlush(display);

    // Start the event loop, which listens for X Events and invokes their
    // corresponding event handler functions.
    while (1)
    {
        XNextEvent(display, &event);
        for (int i = 0; i < event_handler_map_size; i++)
        {
            if (event_handler_map[i].event_type == event.type)
            {
                event_handler_map[i].event_handler(&event, display, root_window);
            }
        }
    }
}

void register_event_handler(int event_type, EventHandler *event_handler)
{
    EventHandlerMap *buffer = realloc(event_handler_map, (event_handler_map_size + 1) * sizeof(EventHandlerMap));
    if (buffer == NULL)
    {
        perror("Failed to allocate memory for event action map\n");
        exit(EXIT_FAILURE);
    }

    event_handler_map = buffer;

    event_handler_map[event_handler_map_size].event_type = event_type;
    event_handler_map[event_handler_map_size].event_handler = event_handler;

    event_handler_map_size++;
}
