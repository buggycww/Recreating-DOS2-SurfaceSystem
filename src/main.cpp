// Recreating-DOS2-SurfaceSystem.cpp : Defines the entry point for the application.
//

#include <raylib.h>

int main()
{
    const Color darkGreen = { 20, 160, 133, 255 };

    constexpr int screenWidth = 1920;
    constexpr int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Recreating DOS2 Surface System");
    SetTargetFPS(60);

    Camera3D camera = Camera3D();
    camera.position = Vector3(0, 5.f, 5.f);
    camera.target = Vector3();
    camera.up = Vector3(0, 1.f, 0);
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        ClearBackground(WHITE);
        BeginDrawing();
        BeginMode3D(camera);

        DrawGrid(10, 1);

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
}