#pragma once

#include "Scene.hpp"

#include <raylib.h>

/**
 * Title scene
 */
class TitleScene : public Scene
{
private:
    /**
     * Bounding box for the start game button
     */
    Rectangle m_startGameButtonBounds;

public:
    /**
     * @brief Constructor
     */
    TitleScene(SceneManager *sceneManager);

    /**
     * @brief Destructor
     */
    ~TitleScene();

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
};
