#include <math.h>
#include <stdlib.h>
#include "renderer.h"
#include "math3d.h"
#include "canvas.h"

// 1. Vertex Projection Pipeline
vec3_t project_vertex(vec3_t vertex, mat4_t world, mat4_t view, mat4_t proj, 
                     int width, int height) {
    // Transform through pipeline stages
    vec4_t v_local = {vertex.x, vertex.y, vertex.z, 1.0f};
    vec4_t v_world = mat4_mul(world, v_local);
    vec4_t v_view = mat4_mul(view, v_world);
    vec4_t v_proj = mat4_mul(proj, v_view);

    // Perspective divide with safety check
    if (fabs(v_proj.w) > 1e-6f) {
        v_proj.x /= v_proj.w;
        v_proj.y /= v_proj.w;
        v_proj.z /= v_proj.w;
    }

    // Convert to screen coordinates (flip Y-axis)
    return (vec3_t){
        (v_proj.x + 1.0f) * 0.5f * width,
        (1.0f - v_proj.y) * 0.5f * height,
        v_proj.z  // Preserve depth for sorting
    };
}

// 2. Circular Viewport Clipping
bool clip_to_circular_viewport(canvas_t* canvas, float x, float y) {
    const float center_x = canvas->width / 2.0f;
    const float center_y = canvas->height / 2.0f;
    const float radius = fminf(canvas->width, canvas->height) / 2.0f;
    
    const float dx = x - center_x;
    const float dy = y - center_y;
    return (dx*dx + dy*dy) <= (radius*radius);
}

// 3. Wireframe Rendering with Depth Sorting
// Depth comparison function for qsort
static int compare_edges(const void* a, const void* b) {
    const edge_depth_t* ea = a;
    const edge_depth_t* eb = b;
    return (ea->depth < eb->depth) - (ea->depth > eb->depth); // Back-to-front
}

void render_wireframe(canvas_t* canvas, const mesh_t* mesh,
                     mat4_t world, mat4_t view, mat4_t proj) {
    edge_depth_t* edges = malloc(mesh->num_edges * sizeof(edge_depth_t));
    
    // Project all edges and calculate depths
    for (int i = 0; i < mesh->num_edges; i++) {
        vec3_t v0 = project_vertex(mesh->edges[i].v0, world, view, proj,
                                  canvas->width, canvas->height);
        vec3_t v1 = project_vertex(mesh->edges[i].v1, world, view, proj,
                                  canvas->width, canvas->height);
        
        edges[i] = (edge_depth_t){
            .v0 = v0, 
            .v1 = v1,
            .depth = (v0.z + v1.z) / 2.0f  // Average depth
        };
    }

    // Sort edges by depth (far to near)
    qsort(edges, mesh->num_edges, sizeof(edge_depth_t), compare_edges);

    // Draw visible edges
    for (int i = 0; i < mesh->num_edges; i++) {
        vec3_t p0 = edges[i].v0;
        vec3_t p1 = edges[i].v1;
        
        // Basic line clipping (checks both endpoints)
        if (clip_to_circular_viewport(canvas, p0.x, p0.y) &&
            clip_to_circular_viewport(canvas, p1.x, p1.y)) {
            draw_line_f(canvas, p0.x, p0.y, p1.x, p1.y, 1.0f);
        }
    }
    
    free(edges);
}
