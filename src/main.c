//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-03-23 15:06:32
//

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

const int WIDTH = 1280;
const int HEIGHT = 720;

const float BIRD_MASS = 0.8f; // Mass of the bird (kg)
const float GRAVITY = 9.81f; // Gravitational constant (m/s²)
const float SLINGSHOT_RIGIDITY = 10.0f; // Rigidity of the spring (N/m)
const float FRICTION = 0.25f; // Friction divided by mass coefficient

void LinearSpace(float start, float end, int num, float* result)
{
    if (num < 1) return;
    
    double step = (num > 1) ? (end - start) / (num - 1) : 0.0f;
    for (int i = 0; i < num; i++) {
        result[i] = start + i * step;
    }
}

float InitialVelocity(float alphaRadians, float l1)
{
    float omega = sqrt(SLINGSHOT_RIGIDITY / BIRD_MASS);
    float v_eject = l1 * omega * sqrt(1 - pow(((BIRD_MASS * GRAVITY) / (l1 * SLINGSHOT_RIGIDITY) * sin(alphaRadians)), 2));
    return v_eject;
}

void ThrowBirdFriction(float alpha, float l1, int numValues, Vector2* out, Vector2 start)
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

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Oiseaux Pas Content");
    Texture2D bird = LoadTexture("assets/red.png");

    SetTargetFPS(144);

    Vector2 originalPoint = { 0, 0 };
    int startDrag = 0;
    Vector2 debugList[100];
    memset(debugList, 0, sizeof(debugList));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 mousePos = GetMousePosition();
        Vector2 pos = { 200.0f, 500.0f };
        Vector2 birdCenter = { pos.x + ((bird.width * 0.1f) / 2.0f), pos.y + ((bird.height * 0.1f) / 2.0f) };
        DrawTextureEx(bird, pos, 0, .1f, WHITE);

        Rectangle rect = { pos.x, pos.y, (float)bird.width, (float)bird.height };
        if (CheckCollisionPointRec(mousePos, rect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                originalPoint = birdCenter;
                startDrag = 1;
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && startDrag) {
            DrawLineEx(originalPoint, mousePos, 5.0f, BLUE);

            // Calculate the l1 vector
            Vector2 sqrtee = { powf(originalPoint.x - mousePos.x, 2.0f), powf(originalPoint.y - mousePos.y, 2.0f) };
            
            // Calculate the incline angle
            Vector2 diff = { mousePos.x - originalPoint.x, mousePos.y - originalPoint.y };
            
            // Slingshot vector
            Vector2 u = { -1.0f, 0.0f };

            // Normalize vector to solve dot product for cos(alpha)
            float length = sqrtf(diff.x * diff.x + diff.y * diff.y);
            Vector2 v = { diff.x / length, diff.y / length };

            // Calculate dot product
            float dotProduct = (u.x * v.x) + (u.y * v.y);

            // Get angle and L1
            float angleRadians = acosf(dotProduct);
            float l1 = sqrt(sqrtee.x + sqrtee.y) / 2.0f;

            // Fill the vector list
            ThrowBirdFriction(angleRadians, l1, 100, debugList, birdCenter);
            for (int i = 0; i < 99; i++) {
                DrawLineEx(debugList[i], debugList[i + 1], 5.0f, GREEN);
            }
        } else {
            startDrag = 0;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
