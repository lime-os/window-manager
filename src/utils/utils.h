#pragma once
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
unsigned long rgb_to_hex(double r, double g, double b);

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
unsigned long rgba_to_hex(double r, double g, double b, double a);

/**
 * @brief Converts a hexadecimal color value to RGB components.
 *
 * @param hex The hexadecimal representation of the color.
 * @param r Pointer to the red channel, where the result will be stored, in the range [0.0, 1.0].
 * @param g Pointer to the green channel, where the result will be stored, in the range [0.0, 1.0].
 * @param b Pointer to the blue channel, where the result will be stored, in the range [0.0, 1.0].
 */
void hex_to_rgb(unsigned long hex, double *r, double *g, double *b);

/**
 * @brief Converts a hexadecimal color value to RGBA components.
 *
 * @param hex The hexadecimal representation of the color.
 * @param r Pointer to the red channel, where the result will be stored, in the range [0.0, 1.0].
 * @param g Pointer to the green channel, where the result will be stored, in the range [0.0, 1.0].
 * @param b Pointer to the blue channel, where the result will be stored, in the range [0.0, 1.0].
 * @param a Pointer to the alpha channel, where the result will be stored, in the range [0.0, 1.0].
 */
void hex_to_rgba(unsigned long hex, double *r, double *g, double *b, double *a);

/**
 * Expands a path by replacing the leading tilde (~) with the user's home directory.
 *
 * @param path A pointer to the null-terminated string representing the path to be expanded.
 * @param expanded_path A pointer to the buffer where the expanded path will be stored.
 * @param size The size of the buffer pointed to by expanded_path.
 *
 * @return 0 on success, or -1 if an error occurs (e.g. HOME env var not set or buffer too small).
 */
int expand_path(const char *path, char *expanded_path, size_t size);

/**
 * Returns the smaller of two integers.
 *
 * @param a First integer to compare.
 * @param b Second integer to compare.
 *
 * @return The smaller of a and b.
 */
int min(int a, int b);

/**
 * Returns the larger of two integers.
 *
 * @param a First integer to compare.
 * @param b Second integer to compare.
 *
 * @return The larger of a and b.
 */
int max(int a, int b);
