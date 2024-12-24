#include "../all.h"

void setup_ewmh_identification_chain(Display *display, Window root_window)
{
    Atom net_supporting_wm_check = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);

    // Create a hidden check window, this window will be persistent throughout
    // the window manager's entire lifecycle.
    Window check_window = XCreateSimpleWindow(display, root_window, -1, -1, 1, 1, 0, 0, 0);

    // Set the _NET_SUPPORTING_WM_CHECK property on the check window, pointing 
    // to itself. This creates a verification loop required by EWMH specification.
    XChangeProperty(
        display,                        // Display
        check_window,                   // Window
        net_supporting_wm_check,        // Property
        XA_WINDOW,                      // Type
        32,                             // Format (32-bit)
        PropModeReplace,                // Mode
        (unsigned char *)&check_window, // Property Data
        1                               // Element Count
    );

    // Set the _NET_SUPPORTING_WM_CHECK property on the root window pointing to 
    // our check window. This completes the identification chain, allowing 
    // clients to verify our window manager presence.
    XChangeProperty(
        display,                        // Display
        root_window,                    // Window
        net_supporting_wm_check,        // Property
        XA_WINDOW,                      // Type
        32,                             // Format (32-bit)
        PropModeReplace,                // Mode
        (unsigned char *)&check_window, // Property Data
        1                               // Element Count
    );

    // Set the _NET_WM_NAME property on the check window to identify the window
    // manager. This is the name that applications will see when querying the
    // window manager name.
    const char *wm_name = "LimeOS Window Manager";
    XChangeProperty(
        display,                        // Display
        check_window,                   // Window
        net_wm_name,                    // Property
        utf8_string,                    // Type
        8,                              // Format (8-bit)
        PropModeReplace,                // Mode
        (unsigned char *)wm_name,       // Property Data
        strlen(wm_name)                 // Element Count
    );
}

void setup_ewmh_supported_list(Display *display, Window root_window)
{
    Atom net_supported = XInternAtom(display, "_NET_SUPPORTED", False);
    Atom net_supporting_wm_check = XInternAtom(display, "_NET_SUPPORTING_WM_CHECK", False);
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom net_client_list = XInternAtom(display, "_NET_CLIENT_LIST", False);

    // Define a list of supported EWMH features.
    const int supported_feature_count = 3;
    Atom supported_features[] = {
        net_supporting_wm_check,
        net_wm_name,
        net_client_list
    };

    // Set the _NET_SUPPORTED property on the root window, listing all the
    // EWMH features that our window manager supports.
    XChangeProperty(
        display,                                // Display
        root_window,                            // Window
        net_supported,                          // Property
        XA_ATOM,                                // Type
        32,                                     // Format (32-bit)
        PropModeReplace,                        // Mode
        (unsigned char *)supported_features,    // Property Data
        supported_feature_count                 // Element Count
    );
}
