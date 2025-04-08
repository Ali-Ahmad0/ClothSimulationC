#include "vec2.h"
#include <math.h>

// Add b to a
void Vec2Add(Vector2* a, Vector2 b) {
	a->x += b.x;
	a->y += b.y;
}

// Subtract b from a
void Vec2Sub(Vector2* a, Vector2 b) {
	a->x -= b.x;
	a->y -= b.y;
}

// Scale a by some value
void Vec2Mul(Vector2* v, float val) {
	v->x *= val;
	v->y *= val;
}

// Scale a by reciprocal of some value
void Vec2Div(Vector2* v, float val) {
	v->x /= val;
	v->y /= val;
}

// Return dot product of 2 vectors
float Vec2Dot(Vector2 a, Vector2 b) {
	return a.x * b.x + a.y * b.y;
}

// Return magnitude of a vector
float Vec2Mag(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

// Return squared magnitude of a vector
float Vec2MagSq(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}