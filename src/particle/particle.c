#include "particle.h"

void VerletIntegrate(Particle* particle, Vector2 accel, float dt) {
	Vector2 new_pos = (Vector2){ .x = 0, .y = 0 };

	// x(t + dt) = 2x(t) - x(t - dt) + (a(t) * dt * dt)
	Vec2Add(&new_pos, particle->curr_pos);
	Vec2Add(&new_pos, particle->curr_pos);
	Vec2Sub(&new_pos, particle->prev_pos);

	Vector2 accel_term = accel;
	Vec2Mul(&accel_term, dt * dt);
	Vec2Add(&new_pos, accel_term);

	particle->prev_pos = particle->curr_pos;
	particle->curr_pos = new_pos;
}
