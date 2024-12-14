#pragma once
#include "../all.h"

unsigned long rgb_to_hex(double r, double g, double b);
unsigned long rgba_to_hex(double r, double g, double b, double a);

void hex_to_rgb(unsigned long hex, double *r, double *g, double *b);
void hex_to_rgba(unsigned long hex, double *r, double *g, double *b, double *a);

int expand_path(const char *path, char *expanded_path, size_t size);

int min(int a, int b);
int max(int a, int b);
