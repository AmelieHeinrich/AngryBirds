//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-03-23 16:41:01
//

#include "physics.h"
#include "constants.h"
#include "math_utils.h"

#include <math.h>
#include <stdlib.h>

float InitialVelocity(float alphaRadians, float l1)
{
    float omega = sqrt(SLINGSHOT_RIGIDITY / BIRD_MASS);
    float v_eject = l1 * omega * sqrt(1 - pow(((BIRD_MASS * GRAVITY) / (l1 * SLINGSHOT_RIGIDITY) * sin(alphaRadians)), 2));
    return v_eject;
}

void GetPredictedTrajectory(float alpha, float l1, int numValues, Vector2* out, Vector2 start)
{
    float v0 = InitialVelocity(alpha, l1);
    float t_impact = (2 * v0 * sin(alpha)) / GRAVITY;

    float* linearSpace = (float*)malloc(sizeof(float) * numValues);
    LinearSpace(0, t_impact, numValues, linearSpace);

    for (int i = 0; i < numValues; i++) {
        float step = linearSpace[i];

        float lambda_xt = v0 * cos(alpha);
        float lambda_yt = v0 * sin(alpha);

        float x_t = (lambda_xt / FRICTION) * (1 - expf(-FRICTION * step));
        float y_t = (lambda_yt / FRICTION) * (1 - expf(-FRICTION * step)) - (GRAVITY / FRICTION) * step;

        out[i].x = start.x + x_t;
        out[i].y = start.y - y_t;
    }

    free(linearSpace);
}
