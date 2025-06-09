#include "../include/canvas.h"
#include "../include/math3d.h"
#include "../include/renderer.h"
#include <stdio.h>
#include <math.h>

// Simple cube mesh for pipeline test
static const vec3_t cube_vertices[8] = {
    { -1, -1, -1 }, { 1, -1, -1 },
    {  1,  1, -1 }, { -1,  1, -1 },
    { -1, -1,  1 }, { 1, -1,  1 },
    {  1,  1,  1 }, { -1,  1,  1 }
};

static const int cube_edges[12][2] = {
    {0,1},{1,2},{2,3},{3,0}, // bottom
    {4,5},{5,6},{6,7},{7,4}, // top
    {0,4},{1,5},{2,6},{3,7}  // sides
};

int main() {
    // Create canvas
    canvas_t* canvas = canvas_create(800, 600);

    // Set up transformation matrices
    mat4_t model, view, proj, mvp;
    mat4_identity(&model);
    mat4_rotate_xyz(&model, 0.7f, 0.7f, 0.0f);

    mat4_identity(&view);
    mat4_translate(&view, 0.0f, 0.0f, -5.0f);

    mat4_frustum_asymmetric(&proj, -1, 1, -0.75, 0.75, 1.0f, 10.0f);

    mat4_t temp;
    mat4_multiply(&temp, &view, &model);
    mat4_multiply(&mvp, &proj, &temp);

    // Project and draw edges
    for (int i = 0; i < 12; ++i) {
        vec3_t v0 = cube_vertices[cube_edges[i][0]];
        vec3_t v1 = cube_vertices[cube_edges[i][1]];

        // Project vertices
        float x0, y0, x1, y1;
        project_vertex(&mvp, &v0, canvas->width, canvas->height, &x0, &y0);
        project_vertex(&mvp, &v1, canvas->width, canvas->height, &x1, &y1);

        // Draw edge
        draw_line_f(canvas, x0, y0, x1, y1, 2.0f);
    }

    // Save result
    canvas_save_ppm(canvas, "pipeline_test.ppm");
    canvas_destroy(canvas);

    printf("Pipeline test completed. Output saved to pipeline_test.ppm\n");
    return 0;
}
