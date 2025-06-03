#include <stdlib.h>
#include "canvas.h"

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
