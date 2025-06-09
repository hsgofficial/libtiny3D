#include "animation.h"
#include "math3d.h"

// Cubic Bézier interpolation
vec3_t vec3_bezier(vec3_t p0, vec3_t p1, vec3_t p2, vec3_t p3, float t) {
    float u = 1.0f - t;
    float tt = t*t;
    float uu = u*u;
    float uuu = uu*u;
    float ttt = tt*t;

    vec3_t point = vec3_mul_f(p0, uuu);
    point = vec3_add(point, vec3_mul_f(p1, 3*uu*t));
    point = vec3_add(point, vec3_mul_f(p2, 3*u*tt));
    point = vec3_add(point, vec3_mul_f(p3, ttt));
    
    return point;
}

// Update animation state
void update_animation(animation_t* anim, float delta_time) {
    anim->time += delta_time;
    
    // Loop animation
    if(anim->time > 1.0f) {
        anim->time -= 1.0f;
    }
    
    // Calculate current position
    anim->current_pos = vec3_bezier(anim->p0, anim->p1, anim->p2, anim->p3, 
                                   anim->time);
}

// Initialize synchronized animation group
void init_animation_group(animation_group_t* group, int capacity) {
    group->animations = malloc(capacity * sizeof(animation_t));
    group->count = 0;
    group->capacity = capacity;
    group->global_time = 0.0f;
}
