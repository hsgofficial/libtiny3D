#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"  // For vec3_t/mat4_t definitions
#include "canvas.h"  // For canvas_t

// Structure for representing edges between vertices
typedef struct {
    vec3_t v0;
    vec3_t v1;
} edge_t;

// Complete mesh structure
typedef struct {
    edge_t* edges;
    int num_edges;
} mesh_t;

// Structure for depth-sorted edges
typedef struct {
    vec3_t v0;
    vec3_t v1;
    float depth;
} edge_depth_t;

// Function declarations
vec3_t project_vertex(vec3_t vertex, mat4_t world, mat4_t view, mat4_t proj, 
                      int width, int height);
bool clip_to_circular_viewport(canvas_t* canvas, float x, float y);
void render_wireframe(canvas_t* canvas, const mesh_t* mesh,
                     mat4_t world, mat4_t view, mat4_t proj);

#endif // RENDERER_H
