#include "../all.h"

/**
 * @brief Converts RGB values to a hexadecimal representation.
 *
 * @param r Red channel, in the range [0.0, 1.0].
 * @param g Green channel, in the range [0.0, 1.0].
 * @param b Blue channel, in the range [0.0, 1.0].
 *
 * @return The hexadecimal representation of the RGB color.
 */
unsigned long rgb_to_hex(double r, double g, double b)
{
    unsigned long hex = 0;
    hex |= ((int)(r * 255) & 0xFF) << 16;
    hex |= ((int)(g * 255) & 0xFF) << 8;
    hex |= ((int)(b * 255) & 0xFF);
    return hex;
}

/**
 * @brief Converts RGBA values to a hexadecimal representation.
 *
 * @param r Red channel, in the range [0.0, 1.0].
 * @param g Green channel, in the range [0.0, 1.0].
 * @param b Blue channel, in the range [0.0, 1.0].
 * @param a Alpha channel, in the range [0.0, 1.0].
 *
 * @return The hexadecimal representation of the RGBA color.
 */
unsigned long rgba_to_hex(double r, double g, double b, double a)
{
    unsigned long hex = 0;
    hex |= ((int)(r * 255) & 0xFF) << 24;
    hex |= ((int)(g * 255) & 0xFF) << 16;
    hex |= ((int)(b * 255) & 0xFF) << 8;
    hex |= ((int)(a * 255) & 0xFF);
    return hex;
}

/**
 * @brief Converts a hexadecimal color value to RGB components.
 *
 * @param hex The hexadecimal representation of the color.
 * @param r Pointer to the red channel, where the result will be stored, in the range [0.0, 1.0].
 * @param g Pointer to the green channel, where the result will be stored, in the range [0.0, 1.0].
 * @param b Pointer to the blue channel, where the result will be stored, in the range [0.0, 1.0].
 */
void hex_to_rgb(unsigned long hex, double *r, double *g, double *b)
{
    *r = ((hex >> 16) & 0xFF) / 255.0;
    *g = ((hex >> 8) & 0xFF) / 255.0;
    *b = (hex & 0xFF) / 255.0;
}

/**
 * @brief Converts a hexadecimal color value to RGBA components.
 *
 * @param hex The hexadecimal representation of the color.
 * @param r Pointer to the red channel, where the result will be stored, in the range [0.0, 1.0].
 * @param g Pointer to the green channel, where the result will be stored, in the range [0.0, 1.0].
 * @param b Pointer to the blue channel, where the result will be stored, in the range [0.0, 1.0].
 * @param a Pointer to the alpha channel, where the result will be stored, in the range [0.0, 1.0].
 */
void hex_to_rgba(unsigned long hex, double *r, double *g, double *b, double *a)
{
    *r = ((hex >> 24) & 0xFF) / 255.0;
    *g = ((hex >> 16) & 0xFF) / 255.0;
    *b = ((hex >> 8) & 0xFF) / 255.0;
    *a = (hex & 0xFF) / 255.0;
}

/**
 * Expands a path by replacing the leading tilde (~) with the user's home directory.
 *
 * @param path A pointer to the null-terminated string representing the path to be expanded.
 * @param expanded_path A pointer to the buffer where the expanded path will be stored.
 * @param size The size of the buffer pointed to by expanded_path.
 *
 * @return 0 on success, or -1 if an error occurs (e.g. HOME env var not set or buffer too small).
 */
int expand_path(const char *path, char *expanded_path, size_t size)
{
    if (path == NULL || expanded_path == NULL || size == 0)
        return -1;

    if (path[0] == '~')
    {
        const char *home = getenv("HOME");
        if (!home)
            return -1; // Home environment variable is not available.

        int char_written_count = snprintf(expanded_path, size, "%s%s", home, path + 1);
        if (char_written_count < 0 || (size_t)char_written_count >= size)
            return -1; // snprintf error or buffer too small.
    }
    else
    {
        int char_written_count = snprintf(expanded_path, size, "%s", path);
        if (char_written_count < 0 || (size_t)char_written_count >= size)
            return -1; // snprintf error or buffer too small.
    }

    return 0;
}

/**
 * Returns the smaller of two integers.
 *
 * @param a First integer to compare.
 * @param b Second integer to compare.
 *
 * @return The smaller of a and b.
 */
int min(int a, int b)
{
    return (a < b) ? a : b;
}

/**
 * Returns the larger of two integers.
 *
 * @param a First integer to compare.
 * @param b Second integer to compare.
 *
 * @return The larger of a and b.
 */
int max(int a, int b)
{
    return (a > b) ? a : b;
}
