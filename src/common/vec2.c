#include "vec2.h"

void Vec2Add(Vector2* a, Vector2 b) {
	a->x += b.x;
	a->y += b.y;
}

void Vec2Sub(Vector2* a, Vector2 b) {
	a->x -= b.x;
	a->y -= b.y;
}

void Vec2Mul(Vector2* a, float val) {
	a->x *= val;
	a->y *= val;
}

void Vec2Div(Vector2* a, float val) {
	a->x /= val;
	a->y /= val;
}
