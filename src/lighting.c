#include "lighting.h"
#include "math3d.h"

// Calculate edge intensity using Lambert's cosine law
float calculate_lambert_intensity(vec3_t edge_dir, vec3_t light_dir) {
    vec3_t norm_edge = vec3_normalize_fast(edge_dir);
    vec3_t norm_light = vec3_normalize_fast(light_dir);
    float dot_product = vec3_dot(norm_edge, norm_light);
    return fmaxf(0.0f, dot_product);
}

// Apply lighting to wireframe edges
void apply_lighting(mesh_t* mesh, light_t lights[], int num_lights) {
    for(int i = 0; i < mesh->num_edges; i++) {
        vec3_t edge_vector = vec3_sub(mesh->edges[i].v1, mesh->edges[i].v0);
        float total_intensity = 0.0f;
        
        // Accumulate light contributions
        for(int j = 0; j < num_lights; j++) {
            total_intensity += calculate_lambert_intensity(edge_vector, 
                                                         lights[j].direction) 
                             * lights[j].intensity;
        }
        
        // Store intensity for rendering
        mesh->edges[i].intensity = fminf(total_intensity, 1.0f);
    }
}
