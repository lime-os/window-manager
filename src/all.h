#pragma once

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/extensions/XInput2.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "limits.h"

#include "errors/errors.h"
#include "atoms/atoms.h"
#include "config/config.h"
#include "background/background.h"

#include "utils/utils.h"
#include "utils/xlib.h"

#include "events/events.h"
#include "events/handlers.h"
#include "events/xinput.h"

#include "portals/portals.h"
#include "portals/dragging.h"
#include "portals/focus.h"
#include "portals/buttons.h"
#include "portals/frames.h"
#include "portals/clients.h"
#include "portals/resizing.h"
#include "portals/title.h"
