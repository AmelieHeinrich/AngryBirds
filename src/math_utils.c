//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-03-23 16:39:50
//

#include "math_utils.h"

void LinearSpace(float start, float end, int num, float* result)
{
    if (num < 1) return;
    
    double step = (num > 1) ? (end - start) / (num - 1) : 0.0f;
    for (int i = 0; i < num; i++) {
        result[i] = start + i * step;
    }
}
