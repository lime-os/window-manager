#pragma once
#include "../all.h"

typedef void EventHandler(XEvent *event, Display *display, Window root_window);

typedef struct {
    int event_type;
    EventHandler *event_handler;
} EventHandlerMap;

void initialize_event_loop(Display *display, Window root_window);
void register_event_handler(int event_type, EventHandler event_handler);

#define EVENT_HANDLER_PARAMS \
    __attribute__((unused)) XEvent *event, \
    __attribute__((unused)) Display *display, \
    __attribute__((unused)) Window root_window

#define HANDLE_IMPLEMENTATION(type, line, count) \
    void handler_##type##line##count(EVENT_HANDLER_PARAMS); \
    void register_handler_##type##line##count() __attribute__((constructor)); \
    void register_handler_##type##line##count() { \
        register_event_handler(type, &handler_##type##line##count); \
    } \
    void handler_##type##line##count(EVENT_HANDLER_PARAMS)
#define HANDLE_EXPANDED(type, line, count) HANDLE_IMPLEMENTATION(type, line, count)
#define HANDLE(type) HANDLE_EXPANDED(type, __LINE__, __COUNTER__)

// Custom event types.
#define Prepare 128
#define Initialize 129
