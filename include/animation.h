#pragma once

#include "math3d.h"

typedef struct {
    vec3_t p0, p1, p2, p3;  // Control points
    float duration;         // Animation duration in seconds
    float start_time;       // When animation started
    int loop;              // 1 for looping, 0 for one-shot
} bezier_animation_t;

typedef struct {
    vec3_t position;
    vec3_t rotation;
    vec3_t scale;
    bezier_animation_t* pos_anim;
    bezier_animation_t* rot_anim;
    float current_time;
} animated_object_t;

// Core Bézier functions
vec3_t vec3_bezier(const vec3_t* p0, const vec3_t* p1, const vec3_t* p2, const vec3_t* p3, float t);
float bezier_ease_in_out(float t);
float bezier_ease_in(float t);
float bezier_ease_out(float t);

// Animation management
bezier_animation_t* animation_create(const vec3_t* p0, const vec3_t* p1, 
                                   const vec3_t* p2, const vec3_t* p3, 
                                   float duration, int loop);
void animation_destroy(bezier_animation_t* anim);
vec3_t animation_get_position(bezier_animation_t* anim, float current_time);
float animation_get_progress(bezier_animation_t* anim, float current_time);

// Object animation
animated_object_t* animated_object_create(void);
void animated_object_destroy(animated_object_t* obj);
void animated_object_update(animated_object_t* obj, float delta_time);
void animated_object_set_position_animation(animated_object_t* obj, bezier_animation_t* anim);
void animated_object_set_rotation_animation(animated_object_t* obj, bezier_animation_t* anim);

// Timing utilities
float get_time(void);  // Returns current time in seconds
void sync_animations(animated_object_t** objects, int count, float sync_time);
