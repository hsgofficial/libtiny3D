#include <stdlib.h>
#include "canvas.h"
#include <math.h>
#include <stdio.h>

// Create a canvas with the given width and height
canvas_t* create_canvas(int width, int height) {
    canvas_t* canvas = malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;

    // Allocate 2D array for pixel brightness
    canvas->pixels = malloc(sizeof(float*) * height);
    for (int i = 0; i < height; i++) {
        canvas->pixels[i] = calloc(width, sizeof(float)); // init to 0.0
    }

    return canvas;
}

// Free the canvas memory
void free_canvas(canvas_t* canvas) {
    if (canvas) {
        for (int i = 0; i < canvas->height; i++) {
            free(canvas->pixels[i]);
        }
        free(canvas->pixels);
        free(canvas);
    }
}
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float fx = x - x0;
    float fy = y - y0;

    // Bilinear weights
    float w00 = (1 - fx) * (1 - fy);
    float w10 = fx * (1 - fy);
    float w01 = (1 - fx) * fy;
    float w11 = fx * fy;

    // Helper macro to safely set pixel with bounds check
    #define SET_PIXEL_SAFE(xx, yy, value) \
        if ((xx) >= 0 && (xx) < canvas->width && (yy) >= 0 && (yy) < canvas->height) \
            canvas->pixels[yy][xx] += (value);

    SET_PIXEL_SAFE(x0, y0, intensity * w00);
    SET_PIXEL_SAFE(x1, y0, intensity * w10);
    SET_PIXEL_SAFE(x0, y1, intensity * w01);
    SET_PIXEL_SAFE(x1, y1, intensity * w11);

    #undef SET_PIXEL_SAFE
}
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = fmaxf(fabsf(dx), fabsf(dy));
    
    if (length == 0.0f) return;  // Avoid division by zero

    float step_x = dx / length;
    float step_y = dy / length;

    for (float i = 0; i <= length; i++) {
        float x = x0 + i * step_x;
        float y = y0 + i * step_y;

        // Draw square around the point for thickness
        int half = (int)(thickness / 2);
        for (int dx = -half; dx <= half; dx++) {
            for (int dy = -half; dy <= half; dy++) {
                set_pixel_f(canvas, x + dx, y + dy, 1.0f);  // Max brightness
            }
        }
    }
}

