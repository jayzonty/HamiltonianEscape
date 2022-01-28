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

        InitAudioDevice();
        if (IsAudioDeviceReady())
        {
            std::cout << "Audio device loaded successfully!" << std::endl;
        }
        SetMasterVolume(1.0f);

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

        CloseAudioDevice();
        CloseWindow();

        delete gameScene;
        delete titleScene;
    }

    return 0;
}