#pragma once

#include <raylib.h>

#include "resources.h"
#include "grid.h"
#include "player.h"
#include "vec3.h"

class Game
{
public:
    Game();

    void Run();

private:
    void Update(float dt);
    void Input(float dt);
    void Draw();

    // Camera basis, derived from yaw/pitch.
    Vector3 Forward() const;            // where the camera looks (includes pitch)
    Vector3 HorizontalForward() const;  // Forward() flattened onto the XZ plane
    Vector3 Right() const;              // strafe direction

    Camera3D camera;
    float yaw = 0.f;                    // radians, 0 = looking toward -Z
    float pitch = 0.f;                  // radians, + = looking up
    float cameraMoveSpeed = 10.f;       // units per second
    float cameraTurnSpeed = 100.f;      // degrees per second
    float cameraZoomSpeed = 2.f;        // world units per wheel notch

    Resources resources;
    Grid grid;
    Player player;
};

// Runs a single game session: builds the Game, plays until the window closes,
// and destroys it before returning. The Game object lives inside this function,
// so its resources are unloaded while the OpenGL context is still alive.
void RunGame();
