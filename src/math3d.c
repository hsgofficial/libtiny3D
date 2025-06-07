#include "math3d.h"

// Helper functions
static void vec3_update_spherical(vec3_t* v) {
    v->r = sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
    v->theta = atan2f(v->y, v->x);
    v->phi = acosf(v->z / (v->r + 1e-8f)); // Avoid division by zero
}

static void vec3_update_cartesian(vec3_t* v) {
    v->x = v->r * sinf(v->phi) * cosf(v->theta);
    v->y = v->r * sinf(v->phi) * sinf(v->theta);
    v->z = v->r * cosf(v->phi);
}

// Core implementations
vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v = {.r = r, .theta = theta, .phi = phi};
    vec3_update_cartesian(&v);
    return v;
}

void vec3_normalize_fast(vec3_t* v) {
    float inv_len = 1.0f / sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
    v->x *= inv_len;
    v->y *= inv_len;
    v->z *= inv_len;
    vec3_update_spherical(v);
}

vec3_t vec3_slerp(const vec3_t* a, const vec3_t* b, float t) {
    float dot = a->x*b->x + a->y*b->y + a->z*b->z;
    float theta = acosf(fminf(fmaxf(dot, -1.0f), 1.0f));
    float sin_theta = sinf(theta);
    
    vec3_t result;
    if (sin_theta < 1e-4) {
        result.x = a->x + t*(b->x - a->x);
        result.y = a->y + t*(b->y - a->y);
        result.z = a->z + t*(b->z - a->z);
    } else {
        float a_coeff = sinf((1-t)*theta)/sin_theta;
        float b_coeff = sinf(t*theta)/sin_theta;
        result.x = a_coeff*a->x + b_coeff*b->x;
        result.y = a_coeff*a->y + b_coeff*b->y;
        result.z = a_coeff*a->z + b_coeff*b->z;
    }
    vec3_normalize_fast(&result);
    return result;
}

// Matrix implementations
void mat4_identity(mat4_t* m) {
    float identity[] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    memcpy(m->m, identity, sizeof(identity));
}

void mat4_translate(mat4_t* m, float tx, float ty, float tz) {
    float translate[] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        tx,ty,tz,1
    };
    memcpy(m->m, translate, sizeof(translate));
}

void mat4_scale(mat4_t* m, float sx, float sy, float sz) {
    float scale[] = {
        sx,0,0,0,
        0,sy,0,0,
        0,0,sz,0,
        0,0,0,1
    };
    memcpy(m->m, scale, sizeof(scale));
}

void mat4_rotate_xyz(mat4_t* m, float rx, float ry, float rz) {
    float cx = cosf(rx), sx = sinf(rx);
    float cy = cosf(ry), sy = sinf(ry);
    float cz = cosf(rz), sz = sinf(rz);

    mat4_t x_rot = {{
        1,0,0,0,
        0,cx,-sx,0,
        0,sx,cx,0,
        0,0,0,1
    }};

    mat4_t y_rot = {{
        cy,0,sy,0,
        0,1,0,0,
        -sy,0,cy,0,
        0,0,0,1
    }};

    mat4_t z_rot = {{
        cz,-sz,0,0,
        sz,cz,0,0,
        0,0,1,0,
        0,0,0,1
    }};

    mat4_t temp;
    mat4_multiply(&temp, &x_rot, &y_rot);
    mat4_multiply(m, &temp, &z_rot);
}

void mat4_frustum_asymmetric(mat4_t* m, float l, float r, float b, float t, float n, float f) {
    m->m[0] = (2*n)/(r-l);
    m->m[5] = (2*n)/(t-b);
    m->m[8] = (r+l)/(r-l);
    m->m[9] = (t+b)/(t-b);
    m->m[10] = -(f+n)/(f-n);
    m->m[11] = -1;
    m->m[14] = -(2*f*n)/(f-n);
    m->m[15] = 0;
}

void mat4_multiply(mat4_t* result, const mat4_t* a, const mat4_t* b) {
    float temp[16];
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            temp[j*4+i] = 0.0f;
            for(int k = 0; k < 4; ++k) {
                temp[j*4+i] += a->m[k*4+i] * b->m[j*4+k];
            }
        }
    }
    memcpy(result->m, temp, sizeof(temp));
}
