#pragma once
#include <raylib.h>
#include "../common/vec2.h"

typedef struct {
	Vector2 curr_pos;
	Vector2 prev_pos;

} Particle;

// Movement using verlet integration
void VerletIntegrate(Particle* particle, Vector2 accel, float dt);
