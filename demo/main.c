#include "include/tiny3d.h"
#include <stdio.h>
#include <math.h>

// Enable/disable specific demos
#define DEMO_TASK1 1
#define DEMO_TASK2 1
#define DEMO_TASK3 1
#define DEMO_TASK4 1

void demo_task1() {
    #if DEMO_TASK1
    printf("\n=== Running Task 1 Demo ===\n");
    canvas_t* canvas = canvas_create(800, 600);
    
    // Draw clock-like lines from center
    float cx = canvas->width/2.0f;
    float cy = canvas->height/2.0f;
    
    for(float angle = 0; angle < 360; angle += 15) {
        float rad = angle * M_PI / 180.0f;
        draw_line_f(canvas, cx, cy, 
                   cx + cosf(rad)*350.0f, 
                   cy + sinf(rad)*350.0f, 
                   1.5f);
    }
    
    canvas_save_ppm(canvas, "task1_demo.ppm");
    canvas_destroy(canvas);
    #endif
}

void demo_task2() {
    #if DEMO_TASK2
    printf("\n=== Running Task 2 Demo ===\n");
    // Create and transform a cube
    mat4_t model, view, projection;
    
    mat4_identity(&model);
    mat4_translate(&model, 2.0f, 3.0f, -5.0f);
    mat4_rotate_xyz(&model, 0.5f, 0.2f, 0.3f);
    
    mat4_identity(&view);
    mat4_translate(&view, 0.0f, 0.0f, -10.0f);
    
    mat4_frustum_asymmetric(&projection, -1, 1, -1, 1, 1, 100);
    
    // Print combined matrix
    mat4_t mvp;
    mat4_multiply(&mvp, &model, &view);
    mat4_multiply(&mvp, &mvp, &projection);
    
    printf("Model-View-Projection Matrix:\n");
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            printf("%.2f ", mvp.m[j*4+i]);
        }
        printf("\n");
    }
    #endif
}

void demo_task3() {
    #if DEMO_TASK3
    printf("\n=== Running Task 3 Demo ===\n");
    canvas_t* canvas = canvas_create(800, 600);
    renderer_t renderer = {
        .canvas = canvas,
        .fov = 60.0f,
        .near = 0.1f,
        .far = 100.0f
    };
    
    // Generate soccer ball mesh
    mesh_t* ball = generate_soccer_ball();
    
    // Animation parameters
    float angle = 0.0f;
    const float rotation_speed = 0.02f;
    
    for(int frame = 0; frame < 100; frame++) {
        canvas_clear(canvas, 0.0f);
        
        // Update rotation
        mat4_rotate_xyz(&ball->transform, 
                       angle, 
                       angle*0.7f, 
                       angle*0.3f);
        angle += rotation_speed;
        
        render_wireframe(&renderer, ball);
        canvas_save_ppm(canvas, "task3_frame%03d.ppm", frame);
    }
    
    mesh_destroy(ball);
    canvas_destroy(canvas);
    #endif
}

void demo_task4() {
    #if DEMO_TASK4
    printf("\n=== Running Task 4 Demo ===\n");
    // Implement lighting and Bezier animation
    // (Add your implementation here)
    #endif
}

int main() {
    demo_task1();
    demo_task2();
    demo_task3();
    demo_task4();
    return 0;
}
