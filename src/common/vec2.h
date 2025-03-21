#pragma once
#include <raylib.h>

// Translation
void Vec2Add(Vector2* a, Vector2 b);
void Vec2Sub(Vector2* a, Vector2 b);

// Scaling
void Vec2Mul(Vector2* v, float val);
void Vec2Div(Vector2* v, float val);

// Dot product
float Vec2Dot(Vector2 a, Vector2 b);

// Magnitude
float Vec2Mag(Vector2 v);
float Vec2MagSq(Vector2 v);