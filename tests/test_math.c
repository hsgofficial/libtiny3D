#include "../include/math3d.h"
#include <stdio.h>

void print_vec3(const vec3_t* v) {
    printf("Cartesian: (%.2f, %.2f, %.2f)\n", v->x, v->y, v->z);
    printf("Spherical: (r=%.2f, θ=%.2frad, φ=%.2frad)\n\n", v->r, v->theta, v->phi);
}

int main() {
    // Test vector conversions
    vec3_t v = vec3_from_spherical(5.0f, M_PI/4, M_PI/6);
    printf("Spherical to Cartesian:\n");
    print_vec3(&v);

    // Test normalization
    vec3_normalize_fast(&v);
    printf("After normalization:\n");
    print_vec3(&v);

    // Test matrix operations
    mat4_t trans, rot, result;
    mat4_translate(&trans, 2.0f, 3.0f, 1.5f);
    mat4_rotate_xyz(&rot, 0.5f, 0.2f, 0.8f);
    mat4_multiply(&result, &trans, &rot);

    printf("Combined transformation matrix:\n");
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            printf("%.2f ", result.m[j*4+i]);
        }
        printf("\n");
    }

    return 0;
}
