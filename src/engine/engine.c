#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include "../common/vec2.h"

Engine EngineCreate() {
    InitWindow(640, 480, "Particle");
    SetTargetFPS(60);

    // Initialize particles array
    Particle* particles = malloc(sizeof(Particle) * INITIAL_SIZE);
    return (Engine) { .particles = particles, .particle_count = 0 };
}

int EngineEvents(Engine* engine) {
    // Check for quit event
    if (WindowShouldClose()) {
        return 1;
    }

    // Check for mouse click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Get mouse position
        Vector2 mouse_pos = GetMousePosition();
        Vector2 pos = (Vector2){ .x = mouse_pos.x, .y = mouse_pos.y };

        // Spawn new particle
        Particle particle = (Particle){ .curr_pos = pos, .prev_pos = pos };

        engine->particle_count++;

        // Particle array resizing
        if (engine->particle_count % INITIAL_SIZE == 0) {
            Particle* temp = realloc(
                engine->particles,
                (engine->particle_count + INITIAL_SIZE) * sizeof(Particle)
            );

            if (!temp) {
                fprintf(stderr, "Unable to resize particles array\n");
                free(engine->particles);
                return -1;
            }

            engine->particles = temp;
            printf("Array resized\n");
        }
        engine->particles[engine->particle_count - 1] = particle;
    }

    return 0;
}

void EngineUpdate(Engine* engine) {
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

            DrawCircle(
                (int)engine->particles[i].curr_pos.x, 
                (int)engine->particles[i].curr_pos.y, 
                METER / 2, WHITE
            );
        }

        EndDrawing();
    }

    // Destroy engine on exit
    EngineDestroy(engine);
}

void EngineDestroy(Engine* engine) {
    // Free particles array
    free(engine->particles);
    CloseWindow();
}