#pragma once

#include "SceneManager.hpp"

/**
 * Class containing data/behavior about a scene
 */
class Scene
{
private:
    /**
     * Reference to the scene manager
     */
    SceneManager *m_sceneManager;

public:
    /**
     * @brief Constructor
     * @param[in] sceneManager Reference to the scene manager
     */
    Scene(SceneManager *sceneManager)
        : m_sceneManager(sceneManager)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~Scene()
    {
    }

    /**
     * @brief Function called when the scene is about to begin
     */
    virtual void Begin() = 0;

    /**
     * @brief Updates the scene
     * @param deltaTime Time elapsed since the previous frame
     */
    virtual void Update(const float& deltaTime) = 0;

    /**
     * @brief Draws the scene
     */
    virtual void Draw() = 0;

    /**
     * @brief Function called when the scene is about to end
     */
    virtual void End() = 0;

protected:
    /**
     * @brief Gets the reference to the scene manager
     * @return Reference to the scene manager
     */
    SceneManager* GetSceneManager()
    {
        return m_sceneManager;
    }
};
