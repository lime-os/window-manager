#pragma once
#include "../all.h"

/**
 * Configures which events the XInput2 extension should listen for on a given
 * window. The function is designed to mimic the XSelectInput function.
 * 
 * @param display The X11 display.
 * @param window The X11 window where the events should be listened for.
 * @param mask Bitmask of XInput2 events to select / listen to.
 * @return 0 if successful, non-zero integer otherwise.
 */
int xi_select_input(Display *display, Window window, long mask);

/**
 * Retrieves the type of a given XInput2 device.
 * 
 * @param display The X11 display.
 * @param device_id The ID of the XInput2 device.
 * @param out_device_type The output parameter to store the device type.
 * @return 0 if successful, non-zero integer otherwise.
 */
int xi_get_device_type(Display *display, int device_id, int *out_device_type);

/**
 * Converts an XInput2 `RawButtonPress` event to a standard X11 `ButtonPress` 
 * event.
 *
 * @param display The X11 display.
 * @param window The X11 window where the event occurred.
 * @param raw_event The XInput2 `RawButtonPress` event to convert.
 * @return The converted standard X11 `ButtonPress` event.
 * 
 * @note Retrieving the `XIRawEvent` data from an XInput2 event is unfortunately
 * not quite straight-forward. XInput2 events present themselves as an X11 
 * `GenericEvent`, containing all its data within the `xcookie` field of the 
 * event struct. One needs to use `XGetEventData(display, &event->xcookie)`
 * to extract this data. Once complete, the data is accessible as follows:
 * `(XIRawEvent)event->xcookie.data`. Don't forget to free the data afterwards
 * using `XFreeEventData` in order to prevent memory leaks.
 */
XEvent xi_convert_raw_button_press_event(Display *display, Window window, XIRawEvent *raw_event);

/**
 * Converts an XInput2 `RawButtonRelease` event to a standard X11
 * `ButtonRelease` event.
 *
 * @param display The X11 display.
 * @param window The X11 window where the event occurred.
 * @param raw_event The XInput2 `RawButtonRelease` event to convert.
 * @return The converted standard X11 `ButtonRelease` event.
 * 
 * @note Retrieving the `XIRawEvent` data from an XInput2 event is unfortunately
 * not quite straight-forward. XInput2 events present themselves as an X11 
 * `GenericEvent`, containing all its data within the `xcookie` field of the 
 * event struct. One needs to use `XGetEventData(display, &event->xcookie)`
 * to extract this data. Once complete, the data is accessible as follows:
 * `(XIRawEvent)event->xcookie.data`. Don't forget to free the data afterwards
 * using `XFreeEventData` in order to prevent memory leaks.
 */
XEvent xi_convert_raw_button_release_event(Display *display, Window window, XIRawEvent *raw_event);

/**
 * Converts an XInput2 `RawMotion` event to a standard X11 `MotionNotify`
 * event.
 *
 * @param display The X11 display.
 * @param window The X11 window where the event occurred.
 * @param raw_event The XInput2 `RawMotion` event to convert.
 * @return The converted standard X11 `MotionNotify` event.
 * 
 * @note Retrieving the `XIRawEvent` data from an XInput2 event is unfortunately
 * not quite straight-forward. XInput2 events present themselves as an X11 
 * `GenericEvent`, containing all its data within the `xcookie` field of the 
 * event struct. One needs to use `XGetEventData(display, &event->xcookie)`
 * to extract this data. Once complete, the data is accessible as follows:
 * `(XIRawEvent)event->xcookie.data`. Don't forget to free the data afterwards
 * using `XFreeEventData` in order to prevent memory leaks.
 */
XEvent xi_convert_raw_motion_event(Display *display, Window window, XIRawEvent *raw_event);
