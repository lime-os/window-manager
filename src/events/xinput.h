#pragma once
#include "../all.h"

int xi_select_input(Display *display, Window window, long mask);
int xi_get_device_type(Display *display, int device_id, int *out_device_type);

XEvent xi_convert_raw_button_press_event(Display *display, Window window, XIRawEvent *raw_event);
XEvent xi_convert_raw_button_release_event(Display *display, Window window, XIRawEvent *raw_event);
XEvent xi_convert_raw_motion_event(Display *display, Window window, XIRawEvent *raw_event);
