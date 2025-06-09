#include "animation.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Core cubic Bézier implementation
vec3_t vec3_bezier(const vec3_t* p0, const vec3_t* p1, const vec3_t* p2, const vec3_t* p3, float t) {
    // Clamp t to [0, 1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    
    float m = 1.0f - t;
    float a = m * m * m;
    float b = 3.0f * m * m * t;
    float c = 3.0f * m * t * t;
    float d = t * t * t;
    
    vec3_t result;
    result.x = a * p0->x + b * p1->x + c * p2->x + d * p3->x;
    result.y = a * p0->y + b * p1->y + c * p2->y + d * p3->y;
    result.z = a * p0->z + b * p1->z + c * p2->z + d * p3->z;
    
    return result;
}

// Predefined easing functions using Bézier curves
float bezier_ease_in_out(float t) {
    // Control points for ease-in-out: (0,0), (0.42,0), (0.58,1), (1,1)
    float m = 1.0f - t;
    return 3.0f * m * m * t * 0.42f + 3.0f * m * t * t * 0.58f + t * t * t;
}

float bezier_ease_in(float t) {
    // Control points for ease-in: (0,0), (0.42,0), (1,1), (1,1)
    float m = 1.0f - t;
    return 3.0f * m * m * t * 0.42f + 3.0f * m * t * t + t * t * t;
}

float bezier_ease_out(float t) {
    // Control points for ease-out: (0,0), (0,0), (0.58,1), (1,1)
    float m = 1.0f - t;
    return 3.0f * m * t * t * 0.58f + t * t * t;
}

// Animation creation and management
bezier_animation_t* animation_create(const vec3_t* p0, const vec3_t* p1, 
                                   const vec3_t* p2, const vec3_t* p3, 
                                   float duration, int loop) {
    bezier_animation_t* anim = malloc(sizeof(bezier_animation_t));
    if (!anim) return NULL;
    
    anim->p0 = *p0;
    anim->p1 = *p1;
    anim->p2 = *p2;
    anim->p3 = *p3;
    anim->duration = duration;
    anim->start_time = get_time();
    anim->loop = loop;
    
    return anim;
}

void animation_destroy(bezier_animation_t* anim) {
    if (anim) {
        free(anim);
    }
}

vec3_t animation_get_position(bezier_animation_t* anim, float current_time) {
    if (!anim) {
        vec3_t zero = {0, 0, 0};
        return zero;
    }
    
    float elapsed = current_time - anim->start_time;
    float t = elapsed / anim->duration;
    
    if (anim->loop) {
        // For looping, use fmod to wrap around
        t = fmodf(t, 1.0f);
        if (t < 0) t += 1.0f;
    } else {
        // Clamp to [0, 1] for one-shot animations
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
    }
    
    return vec3_bezier(&anim->p0, &anim->p1, &anim->p2, &anim->p3, t);
}

float animation_get_progress(bezier_animation_t* anim, float current_time) {
    if (!anim) return 0.0f;
    
    float elapsed = current_time - anim->start_time;
    float t = elapsed / anim->duration;
    
    if (anim->loop) {
        t = fmodf(t, 1.0f);
        if (t < 0) t += 1.0f;
    } else {
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
    }
    
    return t;
}

// Animated object management
animated_object_t* animated_object_create(void) {
    animated_object_t* obj = malloc(sizeof(animated_object_t));
    if (!obj) return NULL;
    
    // Initialize with default values
    obj->position = (vec3_t){0, 0, 0};
    obj->rotation = (vec3_t){0, 0, 0};
    obj->scale = (vec3_t){1, 1, 1};
    obj->pos_anim = NULL;
    obj->rot_anim = NULL;
    obj->current_time = get_time();
    
    return obj;
}

void animated_object_destroy(animated_object_t* obj) {
    if (obj) {
        if (obj->pos_anim) animation_destroy(obj->pos_anim);
        if (obj->rot_anim) animation_destroy(obj->rot_anim);
        free(obj);
    }
}

void animated_object_update(animated_object_t* obj, float delta_time) {
    if (!obj) return;
    
    obj->current_time += delta_time;
    
    // Update position from animation
    if (obj->pos_anim) {
        obj->position = animation_get_position(obj->pos_anim, obj->current_time);
    }
    
    // Update rotation from animation
    if (obj->rot_anim) {
        obj->rotation = animation_get_position(obj->rot_anim, obj->current_time);
    }
}

void animated_object_set_position_animation(animated_object_t* obj, bezier_animation_t* anim) {
    if (obj) {
        if (obj->pos_anim) animation_destroy(obj->pos_anim);
        obj->pos_anim = anim;
    }
}

void animated_object_set_rotation_animation(animated_object_t* obj, bezier_animation_t* anim) {
    if (obj) {
        if (obj->rot_anim) animation_destroy(obj->rot_anim);
        obj->rot_anim = anim;
    }
}

// Timing utilities
float get_time(void) {
    return (float)clock() / CLOCKS_PER_SEC;
}

void sync_animations(animated_object_t** objects, int count, float sync_time) {
    for (int i = 0; i < count; i++) {
        if (objects[i]) {
            objects[i]->current_time = sync_time;
            if (objects[i]->pos_anim) {
                objects[i]->pos_anim->start_time = sync_time;
            }
            if (objects[i]->rot_anim) {
                objects[i]->rot_anim->start_time = sync_time;
            }
        }
    }
}
