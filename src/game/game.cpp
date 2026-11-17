#include "game.h"

#include <cassert>

#include "constants/colors.h"

Game::Game() : resources{}, grid(vec3::Vector3(10, 1, 10), 1.f, 0.05f, resources), player(vec3::Vector3(0.f, 1.3f, 0.f), resources)
{
    assert(IsWindowReady() && "Game must be constructed after InitWindow()");

    camera.Setup(Vector3{ 0.f, 10.f, 10.f }, Vector3{ 0.f, 1.3f, 0.f });

    // Static level geometry never moves, so it is indexed once here.
    collision.build(grid.GetColliders());
    player.SetCollisionWorld(&collision);
    player.SetCamera(&camera.GetCamera());
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();

        Update(dt);

        BeginDrawing();
        ClearBackground(periwinkleGray_02);
        BeginMode3D(camera.GetCamera());
        Draw();
        EndMode3D();
        EndDrawing();
    }
}

void Game::Update(float dt)
{
    camera.Update(dt);
    player.Update(dt);
}

void Game::Draw()
{
    DrawGrid(15, 1);
    grid.Draw();
    player.Draw();
}

void RunGame()
{
    Game game;
    game.Run();
}
