#include <iostream>

#include <raylib.h>

#include "GameScene.hpp"

int main()
{
    {
        InitWindow(800, 600, "Hamiltonian Escape");

        GameScene scene;
        scene.Begin();

        while (!WindowShouldClose())
        {
            float deltaTime = GetFrameTime();
            scene.Update(deltaTime);
            scene.Draw();
        }
        CloseWindow();

        scene.End();
    }

    return 0;
}