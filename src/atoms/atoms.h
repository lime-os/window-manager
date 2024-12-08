#pragma once
#include "../all.h"

extern Atom wm_protocols;
extern Atom wm_delete_window;
extern Atom net_supporting_wm_check;
extern Atom net_wm_name;
extern Atom net_client_list;
extern Atom net_supported;

void initialize_atoms(Display *display, Window root_window);

void add_to_client_list_atom(Display *display, Window root_window, Window window);
void remove_from_client_list_atom(Display *display, Window root_window, Window window);
