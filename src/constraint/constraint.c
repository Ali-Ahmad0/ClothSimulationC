#include "constraint.h"
#include <math.h>

Constraint AddConstraint(Particle* particle_a, Particle* particle_b) {
	// Handle invalid particles
	if (particle_a == NULL || particle_b == NULL) {
		return (Constraint) { NULL, NULL, INFINITY };
	}

	// Initialize and return the constraint
	float initial_distance = sqrtf(
		(particle_a->curr_pos.x - particle_b->curr_pos.x) * (particle_a->curr_pos.x - particle_b->curr_pos.x) +
		(particle_a->curr_pos.y - particle_b->curr_pos.y) * (particle_a->curr_pos.y - particle_b->curr_pos.y)
	);

	return (Constraint) { .particle_a = particle_a, .particle_b = particle_b, .initial_distance = initial_distance };
}

void SatisfyConstraint(Constraint* c) {
    // Skip if both particles are stationary
    if (c->particle_a->stationary && c->particle_b->stationary) 
        return;
        
    // Find distance between particles
    Vector2 delta = c->particle_b->curr_pos;
    Vec2Sub(&delta, c->particle_a->curr_pos);
    float current_distance = Vec2Mag(delta);
    
    // Avoid division by zero
    if (current_distance < 0.0001f) 
        return;
        
    // Compute correction
    float difference = (current_distance - c->initial_distance) / current_distance;
    
    Vector2 correction = delta;
    Vec2Mul(&correction, 0.5f * difference);
    
    // Apply correction based on which particles are stationary
    if (c->particle_a->stationary) {
        Vec2Sub(&c->particle_b->curr_pos, correction);
        Vec2Sub(&c->particle_b->curr_pos, correction); // Double the force
    } 
    else if (c->particle_b->stationary) {
        Vec2Add(&c->particle_a->curr_pos, correction);
        Vec2Add(&c->particle_a->curr_pos, correction); // Double the force
    } 
    else {
        Vec2Add(&c->particle_a->curr_pos, correction);
        Vec2Sub(&c->particle_b->curr_pos, correction);
    }
}