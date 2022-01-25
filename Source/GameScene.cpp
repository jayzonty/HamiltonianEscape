#include "GameScene.hpp"

/**
 * @brief Constructor
 */
GameScene::GameScene()
    : Scene()
{
}

/**
 * @brief Destructor
 */
GameScene::~GameScene()
{
}

/**
 * @brief Function called when the scene is about to begin
 */
void GameScene::Begin()
{
}

/**
 * @brief Updates the scene
 * @param deltaTime Time elapsed since the previous frame
 */
void GameScene::Update(const float& deltaTime)
{
}

/**
 * @brief Draws the scene
 */
void GameScene::Draw()
{
    BeginDrawing();
    ClearBackground(WHITE);
    
    EndDrawing();
}

/**
 * @brief Function called when the scene is about to end
 */
void GameScene::End()
{
}
