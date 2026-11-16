#include "game.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "colors.h"

Game::Game() : resources{}, grid(vec3::Vector3(10, 1, 10), 1.f, 0.05f, resources), player(vec3::Vector3(0.f, 1.5f, 0.f), resources)
{
    assert(IsWindowReady() && "Game must be constructed after InitWindow()");

    // SETUP CAMERA ==================================================
    camera.position = Vector3(0, 10.f, 10.f);
    camera.up = Vector3(0, 1.f, 0);
    camera.fovy = 45.f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Point the camera at the grid origin by deriving yaw/pitch from the
    // direction (0,0,0) - position, so the starting angle matches the position
    // instead of being a separate hard-coded value that can drift out of sync.
    const Vector3 toTarget = {
        -camera.position.x,
        -camera.position.y,
        -camera.position.z
    };

    yaw = std::atan2(toTarget.x, -toTarget.z);
    pitch = std::atan2(toTarget.y, std::sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z));

    const Vector3 forward = Forward();
    camera.target = Vector3{
        camera.position.x + forward.x,
        camera.position.y + forward.y,
        camera.position.z + forward.z
    };
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        Update(dt);

        BeginDrawing();
        ClearBackground(periwinkleGray_02);
        BeginMode3D(camera);
        Draw();
        EndMode3D();
        EndDrawing();
    }
}

void Game::Update(float dt)
{
    Input(dt);

    // Re-aim after the input, so the frame you just moved in and the one you
    // are about to draw use the same yaw/pitch.
    const Vector3 forward = Forward();
    camera.target = Vector3{
        camera.position.x + forward.x,
        camera.position.y + forward.y,
        camera.position.z + forward.z
    };

    player.Update(dt);
}

void Game::Input(float dt)
{
    // ROTATION ------------------------------------------------------
    // Turn first: movement below uses the direction you are facing *now*, so
    // holding forward while turning follows the curve instead of lagging.
    const float turnInput = float(IsKeyDown(KEY_PAGE_DOWN)) - float(IsKeyDown(KEY_PAGE_UP));
    yaw += turnInput * cameraTurnSpeed * DEG2RAD * dt;

    // Extra: Q/E tilt the view up/down, clamped so the camera never flips over.
    const float pitchInput = float(IsKeyDown(KEY_E)) - float(IsKeyDown(KEY_Q));
    pitch = std::clamp(pitch + pitchInput * cameraTurnSpeed * DEG2RAD * dt,
                       -85.f * DEG2RAD, 85.f * DEG2RAD);

    // MOVEMENT ------------------------------------------------------
    const float forwardInput = float(IsKeyDown(KEY_W)) - float(IsKeyDown(KEY_S));
    const float strafeInput = float(IsKeyDown(KEY_D)) - float(IsKeyDown(KEY_A));
    const float verticalInput = float(IsKeyDown(KEY_UP)) - float(IsKeyDown(KEY_DOWN));

    Vector3 move = { 0.f, 0.f, 0.f };

    if (forwardInput != 0.f || strafeInput != 0.f)
    {
        const Vector3 forward = HorizontalForward();
        const Vector3 right = Right();

        move.x = forward.x * forwardInput + right.x * strafeInput;
        move.z = forward.z * forwardInput + right.z * strafeInput;

        // Normalise, so W+D is not ~1.41x faster than W alone.
        const float length = std::sqrt(move.x * move.x + move.z * move.z);
        move.x /= length;
        move.z /= length;
    }

    move.y = verticalInput;

    camera.position.x += move.x * cameraMoveSpeed * dt;
    camera.position.y += move.y * cameraMoveSpeed * dt;
    camera.position.z += move.z * cameraMoveSpeed * dt;

    // ZOOM ----------------------------------------------------------
    const float wheel = GetMouseWheelMove();
    if (wheel != 0.f)
    {
        const Vector3 forward = Forward();
        camera.position.x += forward.x * wheel * cameraZoomSpeed;
        camera.position.y += forward.y * wheel * cameraZoomSpeed;
        camera.position.z += forward.z * wheel * cameraZoomSpeed;

        // Keep the camera above the floor so pitch-zooming doesn't burrow under it.
        camera.position.y = std::max(camera.position.y, 0.5f);
    }
}

void Game::Draw()
{
    DrawGrid(15, 1);
    grid.Draw();
    player.Draw();
}

Vector3 Game::Forward() const
{
    const float cp = std::cos(pitch);
    return Vector3{ std::sin(yaw) * cp, std::sin(pitch), -std::cos(yaw) * cp };
}

Vector3 Game::HorizontalForward() const
{
    return Vector3{ std::sin(yaw), 0.f, -std::cos(yaw) };
}

Vector3 Game::Right() const
{
    return Vector3{ std::cos(yaw), 0.f, std::sin(yaw) };
}

void RunGame()
{
    Game game;
    game.Run();
}
