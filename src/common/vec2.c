#include "vec2.h"
#include <math.h>

void Vec2Add(Vector2* a, Vector2 b) {
	a->x += b.x;
	a->y += b.y;
}

void Vec2Sub(Vector2* a, Vector2 b) {
	a->x -= b.x;
	a->y -= b.y;
}

void Vec2Mul(Vector2* v, float val) {
	v->x *= val;
	v->y *= val;
}

void Vec2Div(Vector2* v, float val) {
	v->x /= val;
	v->y /= val;
}

float Vec2Dot(Vector2 a, Vector2 b) {
	return a.x * b.x + a.y * b.y;
}

float Vec2Mag(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float Vec2MagSq(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}