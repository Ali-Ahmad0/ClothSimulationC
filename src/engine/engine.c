#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include "../common/vec2.h"

void CreateClothGrid(Engine* engine, int rows, int cols, float spacing);

Engine EngineCreate() {
    // Initialize the engine
    InitWindow(640, 480, "Particle");
    SetTargetFPS(60);

    return (Engine) { .particles = NULL, .particle_count = 0, .constraints = NULL, .constraint_count = 0 };
}

int EngineEvents(Engine* engine) {
    // Check for quit event
    if (WindowShouldClose()) {
        return 1;
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
    unsigned int max_constraints = rows * (cols - 1) + (rows - 1) * cols + 2 * (rows - 1) * (cols - 1);
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
    CreateClothGrid(engine, 16, 12, METER * 0.75);

    while (!WindowShouldClose()) {
        if (EngineEvents(engine) != 0) {
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Update and draw particles
        for (unsigned int i = 0; i < engine->particle_count; i++) {
            Vector2 gravity = (Vector2){ .x = 0, .y = 9.81f * METER };

            VerletIntegrate(&engine->particles[i], gravity, GetFrameTime());
        }

        for (int i = 0; i < 24; i++) {
            for (unsigned int i = 0; i < engine->constraint_count; i++) {
                SatisfyConstraint(&engine->constraints[i]);
                DrawLine(
                    (int)engine->constraints[i].particle_a->curr_pos.x,
                    (int)engine->constraints[i].particle_a->curr_pos.y,
                    (int)engine->constraints[i].particle_b->curr_pos.x,
                    (int)engine->constraints[i].particle_b->curr_pos.y,
                    WHITE
                );
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