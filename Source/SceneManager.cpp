#include "SceneManager.hpp"

#include "Scene.hpp"

/**
 * @brief Constructor
 */
SceneManager::SceneManager()
    : m_scenes()
    , m_activeScene(nullptr)
{
}

/**
 * @brief Destructor
 */
SceneManager::~SceneManager()
{
}

/**
 * @brief Gets the currently active scene
 * @return Reference to the currently active scene
 */
Scene* SceneManager::GetActiveScene()
{
    return m_activeScene;
}

/**
 * @brief Associates the specified scene ID to the specified scene
 * @param[in] sceneId Scene ID
 * @param[in] scene Reference to the scene
 * @note The scene manager will NOT free the scene upon destruction. As such,
 * the owner of the scene manager should also be the one to free the scene.
 */
void SceneManager::RegisterScene(const uint32_t &sceneId, Scene *scene)
{
    m_scenes[sceneId] = scene;
}

/**
 * @brief Switches to the scene identified by the specified scene ID
 * @param[in] sceneId Scene ID of the scene to switch to
 */
void SceneManager::SwitchToScene(const uint32_t& sceneId)
{
    if (m_activeScene != nullptr)
    {
        m_activeScene->End();
    }

    if (m_scenes.find(sceneId) != m_scenes.end())
    {
        m_activeScene = m_scenes[sceneId];
        m_activeScene->Begin();
    }
}
