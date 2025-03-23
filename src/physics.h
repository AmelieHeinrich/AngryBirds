//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-03-23 16:40:30
//

#pragma once

#include <raylib.h>

float InitialVelocity(float alphaRadians, float l1);
void GetPredictedTrajectory(float alpha, float l1, int numValues, Vector2* out, Vector2 start);
