#pragma once

/**
 * Class containing data/behavior about a scene
 */
class Scene
{
private:

public:
    /**
     * @brief Constructor
     */
    Scene()
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
};
