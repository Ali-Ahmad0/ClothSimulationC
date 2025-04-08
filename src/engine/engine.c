#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../common/vec2.h"

void CreateClothGrid(Engine* engine, int rows, int cols, float spacing);
float PointSegmentDistanceSquared(Vector2 point, Vector2 a, Vector2 b);

Engine EngineCreate() {
    // Initialize the engine
    InitWindow(1600, 900, "Particle");
    SetTargetFPS(60);

    return (Engine) { .particles = NULL, .particle_count = 0, .constraints = NULL, .constraint_count = 0 };
}

// Point to segment distance algorithm
float PointSegmentDistanceSquared(Vector2 point, Vector2 a, Vector2 b) {
    // Vector from a to b
    Vector2 ab = { b.x - a.x, b.y - a.y };
    
    // Vector from a to the point p
    Vector2 ap = { point.x - a.x, point.y - a.y };

    // Project ap onto ab
    float dot = Vec2Dot(ab, ap);
    float ab_mag_sq = Vec2MagSq(ab);
    float t = fmaxf(0, fminf(1, dot / ab_mag_sq));

    // Closest point on the line segment
    Vector2 closest = { a.x + t * ab.x, a.y + t * ab.y };

    // Distance between the point and the closest point on the line segment
    float dx = point.x - closest.x;
    float dy = point.y - closest.y;
    return dx * dx + dy * dy;
}

int EngineEvents(Engine* engine) {
    // Check for quit event
    if (WindowShouldClose()) {
        return 1;
    }

    // Handle cloth tearing
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_position = GetMousePosition();
        
        // Find the nearest constraint
        float min_distance_sq = INFINITY;
        Constraint* nearest = NULL;

        for (unsigned int i = 0; i < engine->constraint_count; i++) {
            float distance_sq = PointSegmentDistanceSquared(
                mouse_position, 
                engine->constraints[i].particle_a->curr_pos,
                engine->constraints[i].particle_b->curr_pos
            );

            if (distance_sq < min_distance_sq) {
                min_distance_sq = distance_sq;
                nearest = &engine->constraints[i];
            }
        }

        // Deactivate constraint
        if (nearest && sqrtf(min_distance_sq) < 12.0f) {
            nearest->active = 0;
        }
    }

    return 0;
}

// For cloth simulation
void CreateClothGrid(Engine* engine, int rows, int cols, float spacing) {
    // Initialize particles array
    unsigned int total_particles = rows * cols;
    engine->particles = malloc(sizeof(Particle) * total_particles);

    if (!engine->particles) {
        fprintf(stderr, "Unable to initialize particles array");
        return;
    }

    // Initialize constraints array
    unsigned int max_constraints = rows * (cols - 1) + cols * (rows - 1);
    engine->constraints = malloc(sizeof(Constraint) * max_constraints);

    if (!engine->constraints) {
        fprintf(stderr, "Unable to initialize constraints array");
        return;
    }

    // Start position of cloth
    float start_x = (GetScreenWidth() - (cols - 1) * spacing) / 2;
    float start_y = 64;

    // Create particles
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;
            Vector2 position = { start_x + j * spacing, start_y + i * spacing };

            engine->particles[index].curr_pos = position;
            engine->particles[index].prev_pos = position;
            
            // Pin the top row
            engine->particles[index].stationary = (i == 0); 
        }
    }
    engine->particle_count = total_particles;

    // Create constraints
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;

            // Horizontal constraint (right)
            if (j < cols - 1) {
                engine->constraints[engine->constraint_count++] =
                    AddConstraint(&engine->particles[index], &engine->particles[index + 1]);
            }

            // Vertical constraint (down)
            if (i < rows - 1) {
                engine->constraints[engine->constraint_count++] =
                    AddConstraint(&engine->particles[index], &engine->particles[index + cols]);
            }
        }
    }
}

void EngineUpdate(Engine* engine) {
    CreateClothGrid(engine, 60, 40, 12);

    int frames = 0;
    int frames_per_second = 0;

    float elapsed_time = 0.0f;

    while (!WindowShouldClose()) {
        if (EngineEvents(engine) != 0) {
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Render FPS
        frames++;
        float dt = GetFrameTime();
        elapsed_time += dt;

        if (elapsed_time >= 1.0f) {
            frames_per_second = frames;
            frames = 0;
            elapsed_time -= 1.0f;
        }
        //DrawText(TextFormat("FPS: %i", frames_per_second), 16, 16, 32, GREEN);

        // Update and draw particles
        for (unsigned int i = 0; i < engine->particle_count; i++) {
            Vector2 gravity = (Vector2){ .x = 0, .y = 9.81f * METER };

            VerletIntegrate(&engine->particles[i], gravity, GetFrameTime());
        }

        for (int i = 0; i < 32; i++) {
            for (unsigned int i = 0; i < engine->constraint_count; i++) {
                SatisfyConstraint(&engine->constraints[i]);
    
                if (engine->constraints[i].active) {
                    DrawLine(
                        (int)engine->constraints[i].particle_a->curr_pos.x,
                        (int)engine->constraints[i].particle_a->curr_pos.y,
                        (int)engine->constraints[i].particle_b->curr_pos.x,
                        (int)engine->constraints[i].particle_b->curr_pos.y,
                        WHITE
                    );
                }
            }
        }
        EndDrawing();
    }

    // Destroy engine on exit
    EngineDestroy(engine);
}

void EngineDestroy(Engine* engine) {
    // Free the engine memory
    free(engine->particles);
    free(engine->constraints);
    CloseWindow();
}