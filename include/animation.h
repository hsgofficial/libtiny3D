typedef struct {
    vec3_t p0, p1, p2, p3;
    vec3_t current_pos;
    float time;
} animation_t;

typedef struct {
    animation_t* animations;
    int count;
    int capacity;
    float global_time;
} animation_group_t;

vec3_t vec3_bezier(vec3_t p0, vec3_t p1, vec3_t p2, vec3_t p3, float t);
void update_animation(animation_t* anim, float delta_time);
void init_animation_group(animation_group_t* group, int capacity);
