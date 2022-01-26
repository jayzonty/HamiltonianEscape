#include <iostream>

#include "Constants.hpp"
#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "TitleScene.hpp"

#include <raylib.h>

int main()
{
    {
        InitWindow(800, 600, "Hamiltonian Escape");

        SceneManager sceneManager;

        // Create all scenes
        TitleScene *titleScene = new TitleScene(&sceneManager);
        GameScene *gameScene = new GameScene(&sceneManager);

        // Register all scenes
        sceneManager.RegisterScene(Constants::TITLE_SCENE_ID, titleScene);
        sceneManager.RegisterScene(Constants::GAME_SCENE_ID, gameScene);

        // Switch to the starting scene
        sceneManager.SwitchToScene(Constants::TITLE_SCENE_ID);

        while (!WindowShouldClose())
        {
            float deltaTime = GetFrameTime();

            Scene *activeScene = sceneManager.GetActiveScene();
            if (activeScene != nullptr)
            {
                activeScene->Update(deltaTime);
                activeScene->Draw();
            }
        }
        CloseWindow();

        delete gameScene;
        delete titleScene;
    }

    return 0;
}