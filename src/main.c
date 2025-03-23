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

#include "constants.h"
#include "math_utils.h"
#include "physics.h"

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Oiseaux Pas Content");
    Texture2D bird = LoadTexture("assets/red.png");
    Texture2D background = LoadTexture("assets/background.jpg");

    SetTargetFPS(144);

    Vector2 originalPoint = { 0, 0 };
    int startDrag = 0;
    Vector2 debugList[100];
    memset(debugList, 0, sizeof(debugList));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTextureEx(background, {0, 0}, 0, 1, WHITE);
        DrawText("Angry Birds Math 2025 - Amelie Heinrich", 20, 20, 30, BLACK);

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
            GetPredictedTrajectory(angleRadians, l1, 100, debugList, birdCenter);
            for (int i = 0; i < 99; i++) {
                DrawLineEx(debugList[i], debugList[i + 1], 5.0f, GREEN);
            }

            // Draw UI
            char fmt[256];
            sprintf(fmt, "Angle: %.2f\nLength: %.2f", angleRadians * RAD2DEG, l1);
            DrawText(fmt, 20, 50, 30, BLACK);
        } else {
            startDrag = 0;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
