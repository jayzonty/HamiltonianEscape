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
        SetMasterVolume(0.5f);

        Music backgroundMusic = LoadMusicStream("Resources/Audio/bgm.wav");
        backgroundMusic.looping = true;

        // HACK: To make the audio loop more seamless.
        // I figure it's because raylib immediately stops the song
        // upon reaching the end of the buffer before the data can be 
        // sent to the speakers. For now, I just added a big number to the frame count
        // but a more flexbible approach should be thought of in the future
        backgroundMusic.frameCount += 1000;

        PlayMusicStream(backgroundMusic);

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
            UpdateMusicStream(backgroundMusic);

            float deltaTime = GetFrameTime();

            Scene *activeScene = sceneManager.GetActiveScene();
            if (activeScene != nullptr)
            {
                activeScene->Update(deltaTime);
                activeScene->Draw();
            }
        }

        UnloadMusicStream(backgroundMusic);

        CloseAudioDevice();
        CloseWindow();

        delete gameScene;
        delete titleScene;
    }

    return 0;
}