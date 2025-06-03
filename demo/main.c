#include <stdio.h>
#include <math.h>
#include "../include/canvas.h"

#define PI 3.14159265f

int main() {
    int width = 100;
    int height = 100;

    canvas_t* canvas = create_canvas(width, height);
    float cx = width / 2.0f;
    float cy = height / 2.0f;

    // Draw lines every 15 degrees
    for (int angle = 0; angle < 360; angle += 15) {
        float rad = angle * PI / 180.0f;
        float x = cx + cosf(rad) * 40.0f;
        float y = cy + sinf(rad) * 40.0f;
        draw_line_f(canvas, cx, cy, x, y, 1.5f);
    }

    // Debug: Print as ASCII art (or write to file later)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float val = canvas->pixels[y][x];
            putchar(val > 0.5f ? '#' : (val > 0.1f ? '.' : ' '));
        }
        putchar('\n');
    }

    free_canvas(canvas);
    return 0;
}
