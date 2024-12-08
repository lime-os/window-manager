#pragma once
#include "../all.h"

#define CFG_DIRECTORY "~/.config/lime"
#define CFG_FILE_PATH "~/.config/lime/wm.config"

#define CFG_MAX_LINE_LENGTH 256
#define CFG_MAX_KEY_LENGTH 64
#define CFG_MAX_VALUE_LENGTH 64
#define CFG_MAX_ENTRIES 64

#define CFG_TYPE_BACKGROUND_MODE str
#define CFG_KEY_BACKGROUND_MODE "background_mode"
#define CFG_DEF_BACKGROUND_MODE "solid"
#define CFG_BUNDLE_BACKGROUND_MODE CFG_TYPE_BACKGROUND_MODE, CFG_KEY_BACKGROUND_MODE, CFG_DEF_BACKGROUND_MODE

#define CFG_TYPE_BACKGROUND_COLOR hex
#define CFG_KEY_BACKGROUND_COLOR "background_color"
#define CFG_DEF_BACKGROUND_COLOR "0x1C1C1C"
#define CFG_BUNDLE_BACKGROUND_COLOR CFG_TYPE_BACKGROUND_COLOR, CFG_KEY_BACKGROUND_COLOR, CFG_DEF_BACKGROUND_COLOR

#define CFG_TYPE_BACKGROUND_IMAGE_PATH path
#define CFG_KEY_BACKGROUND_IMAGE_PATH "background_image_path"
#define CFG_DEF_BACKGROUND_IMAGE_PATH "~/background.png"
#define CFG_BUNDLE_BACKGROUND_IMAGE_PATH CFG_TYPE_BACKGROUND_IMAGE_PATH, CFG_KEY_BACKGROUND_IMAGE_PATH, CFG_DEF_BACKGROUND_IMAGE_PATH

typedef struct {
    char key[CFG_MAX_KEY_LENGTH];
    char value[CFG_MAX_VALUE_LENGTH];
} ConfigEntry;

void get_config_value_str(char *dest, size_t dest_size, char *key, char *fallback);
void get_config_value_path(char *dest, size_t dest_size, char *key, char *fallback);
void get_config_value_hex(unsigned long *dest, size_t dest_size, char *key, char *fallback);

#define GET_CONFIG_IMPL(dest, dest_size, type, key, fallback) \
    get_config_value_##type(dest, dest_size, key, fallback)

#define GET_CONFIG(dest, dest_size, bundle) \
    GET_CONFIG_IMPL(dest, dest_size, bundle)
