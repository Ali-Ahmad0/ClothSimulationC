#pragma once
#include <raylib.h>
#include "../particle/particle.h"

#define INITIAL_SIZE 128
#define METER 32 // 1 meter = 32px

typedef struct {
    Particle* particles;
    unsigned int particle_count;
} Engine;

Engine EngineCreate();
int EngineEvents(Engine* engine);
void EngineUpdate(Engine* engine);
void EngineDestroy(Engine* engine);