#include "../all.h"

static EventHandlerMap *event_handler_map = NULL;
static int event_handler_map_size = 0;

void register_event_handler(int event_type, EventHandler *event_handler)
{
    EventHandlerMap *buffer = realloc(event_handler_map, (event_handler_map_size + 1) * sizeof(EventHandlerMap));
    if (buffer == NULL)
    {
        // TODO Log error in file.
        perror("Failed to allocate memory for event action map\n");
        exit(EXIT_FAILURE);
    }

    event_handler_map = buffer;

    event_handler_map[event_handler_map_size].event_type = event_type;
    event_handler_map[event_handler_map_size].event_handler = event_handler;

    event_handler_map_size++;
}

void invoke_event_handlers(Display *display, Window window, int event_type, XEvent *event)
{
    for (int i = 0; i < event_handler_map_size; i++)
    {
        if (event_handler_map[i].event_type == event_type)
        {
            event_handler_map[i].event_handler(event, display, window);
        }
    }
}
