#pragma once
#include <raylib.h>

// Translation
void Vec2Add(Vector2* a, Vector2 b);
void Vec2Sub(Vector2* a, Vector2 b);

// Scaling
void Vec2Mul(Vector2* a, float val);
void Vec2Div(Vector2* a, float val);
