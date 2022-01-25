#include <iostream>

#include <raylib.h>

#include "GameScene.hpp"

int main()
{
    {
        GameScene scene;

        InitWindow(800, 600, "Hamiltonian Escape");

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