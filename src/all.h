#pragma once

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "limits.h"

#include "atoms/atoms.h"
#include "events/events.h"
#include "config/config.h"
#include "utils/utils.h"
#include "window/window.h"
#include "background/background.h"
