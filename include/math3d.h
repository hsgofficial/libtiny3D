#pragma once
#include <math.h>

typedef struct {
    // Cartesian coordinates
    float x, y, z;
    // Spherical coordinates
    float r, theta, phi;
} vec3_t;

typedef struct {
    float m[16]; // Column-major 4x4 matrix
} mat4_t;

// Vector functions
vec3_t vec3_from_spherical(float r, float theta, float phi);
void vec3_normalize_fast(vec3_t* v);
vec3_t vec3_slerp(const vec3_t* a, const vec3_t* b, float t);

// Matrix operations
void mat4_identity(mat4_t* m);
void mat4_translate(mat4_t* m, float tx, float ty, float tz);
void mat4_scale(mat4_t* m, float sx, float sy, float sz);
void mat4_rotate_xyz(mat4_t* m, float rx, float ry, float rz);
void mat4_frustum_asymmetric(mat4_t* m, float l, float r, float b, float t, float n, float f);
void mat4_multiply(mat4_t* result, const mat4_t* a, const mat4_t* b);
