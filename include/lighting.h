typedef struct {
    vec3_t direction;
    float intensity;
} light_t;

void apply_lighting(mesh_t* mesh, light_t lights[], int num_lights);
