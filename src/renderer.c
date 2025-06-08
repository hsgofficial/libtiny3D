#include <math.h>
#include <stdlib.h>
#include "renderer.h"
#include "math3d.h"
#include "canvas.h"

// Project vertex through transformation pipeline
vec3_t project_vertex(vec3_t vertex, mat4_t world, mat4_t view, mat4_t proj, int width, int height) {
    vec4_t v_local = {vertex.x, vertex.y, vertex.z, 1.0f};
    
    // Transform through pipeline
    vec4_t v_world = mat4_mul(world, v_local);
    vec4_t v_view = mat4_mul(view, v_world);
    vec4_t v_proj = mat4_mul(proj, v_view);

    // Perspective divide
    if (fabs(v_proj.w) > 1e-6) {
        v_proj.x /= v_proj.w;
        v_proj.y /= v_proj.w;
        v_proj.z /= v_proj.w;
    }

    // Convert to screen coordinates
    return (vec3_t){
        (v_proj.x + 1.0f) * 0.5f * width,
        (1.0f - v_proj.y) * 0.5f * height,  // Flip Y-axis
        v_proj.z  // Store depth for sorting
    };
}

// Circular clipping check
bool clip_to_circular_viewport(canvas_t* canvas, float x, float y) {
    const float center_x = canvas->width / 2.0f;
    const float center_y = canvas->height / 2.0f;
    const float radius = fminf(canvas->width, canvas->height) / 2.0f;
    
    const float dx = x - center_x;
    const float dy = y - center_y;
    return (dx*dx + dy*dy) <= (radius*radius);
}

// Depth comparison for sorting
static int compare_edges(const void* a, const void* b) {
    const edge_depth_t* ea = (const edge_depth_t*)a;
    const edge_depth_t* eb = (const edge_depth_t*)b;
    return (ea->depth < eb->depth) - (ea->depth > eb->depth);
}

// Wireframe rendering with depth sorting
void render_wireframe(canvas_t* canvas, const mesh_t* mesh, 
                     mat4_t world, mat4_t view, mat4_t proj) {
    const int num_edges = mesh->num_edges;
    edge_depth_t* edges = malloc(num_edges * sizeof(edge_depth_t));

    // Project all edges and calculate depths
    for (int i = 0; i < num_edges; i++) {
        vec3_t v0 = project_vertex(mesh->edges[i].v0, world, view, proj, 
                                  canvas->width, canvas->height);
        vec3_t v1 = project_vertex(mesh->edges[i].v1, world, view, proj,
                                  canvas->width, canvas->height);
        
        edges[i].v0 = v0;
        edges[i].v1 = v1;
        edges[i].depth = (v0.z + v1.z) / 2.0f;  // Average depth
    }

    // Sort edges back-to-front
    qsort(edges, num_edges, sizeof(edge_depth_t), compare_edges);

    // Draw visible edges
    for (int i = 0; i < num_edges; i++) {
        const vec3_t v0 = edges[i].v0;
        const vec3_t v1 = edges[i].v1;

        if (clip_to_circular_viewport(canvas, v0.x, v0.y) &&
            clip_to_circular_viewport(canvas, v1.x, v1.y)) {
            draw_line_f(canvas, v0.x, v0.y, v1.x, v1.y, 1.0f);
        }
    }

    free(edges);
}
