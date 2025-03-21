#pragma once
#include "../particle/particle.h"

// Make sure particles remain at same distance from each other
typedef struct {
	Particle* particle_a;
	Particle* particle_b;

	float initial_distance;

	int active;
} Constraint;

Constraint AddConstraint(Particle* particle_a, Particle* particle_b);
void SatisfyConstraint(Constraint* c);