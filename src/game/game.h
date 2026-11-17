#pragma once

#include "camera/freeCameraController.h"
#include "collision detection/CollisionWorld.h"
#include "player/player.h"
#include "structs/resources.h"
#include "surface/grid.h"

class Game
{
public:
    Game();

    void Run();

private:
    void Update(float dt);
    void Draw();

    FreeCameraController camera;

    Resources resources;
    Grid grid;
    CollisionWorld collision;
    Player player;
};

// Runs a single game session: builds the Game, plays until the window closes,
// and destroys it before returning. The Game object lives inside this function,
// so its resources are unloaded while the OpenGL context is still alive.
void RunGame();
