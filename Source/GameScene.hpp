#pragma once

#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "Scene.hpp"

/**
 * @brief Game scene
 */
class GameScene : public Scene
{
private:

public:
    /**
     * @brief Constructor
     */
    GameScene();

    /**
     * @brief Destructor
     */
    ~GameScene();

    /**
     * @brief Function called when the scene is about to begin
     */
    void Begin() override;

    /**
     * @brief Updates the scene
     * @param deltaTime Time elapsed since the previous frame
     */
    void Update(const float& deltaTime) override;

    /**
     * @brief Draws the scene
     */
    void Draw() override;

    /**
     * @brief Function called when the scene is about to end
     */
    void End() override;

private:
};
