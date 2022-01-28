#pragma once

#include "LevelData.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"

#include <vector>

#include <raylib.h>
#include <raymath.h>

/**
 * @brief Game scene
 */
class GameScene : public Scene
{
private:
    /**
     * State
     */
    enum class State
    {
        Play,

        StartRoom,
        EndRoom,

        StartLevel,
        EndLevel,

        GameEnd
    };

private:
    /**
     * Current state
     */
    State m_currentState;

    /**
     * List of level data
     */
    std::vector<LevelData> m_levels;

    /**
     * Current level
     */
    int32_t m_currentLevelIndex;

    /**
     * Current room
     */
    int32_t m_currentRoomIndex;

    /**
     * Current X-position of the player
     */
    int32_t m_playerPositionX;

    /**
     * Current Y-position of the player
     */
    int32_t m_playerPositionY;

    /**
     * Scale of the player's visual
     */
    float m_playerVisualScale;

    /**
     * Timer for the start of the level
     */
    float m_startLevelTimer;

    /**
     * Timer for the start of level fade in
     */
    float m_startLevelFadeInTimer;

    /**
     * Timer for the end of level fade out
     */
    float m_endLevelFadeOutTimer;

    /**
     * Reset button bounds
     */
    Rectangle m_resetButtonBounds;

    /**
     * Back to title button bounds
     */
    Rectangle m_backToTitleButton;

    /**
     * List of accepted keys to make the player move up
     */
    std::vector<uint32_t> m_moveUpKeys;

    /**
     * List of accepted keys to make the player move down
     */
    std::vector<uint32_t> m_moveDownKeys;

    /**
     * List of accepted keys to make the player move left
     */
    std::vector<uint32_t> m_moveLeftKeys;

    /**
     * List of accepted keys to make the player move right
     */
    std::vector<uint32_t> m_moveRightKeys;

    /**
     * List of accepted keys to reset the current room
     */
    std::vector<uint32_t> m_resetRoomKeys;

    Music m_backgroundMusic;

public:
    /**
     * @brief Constructor
     */
    GameScene(SceneManager *sceneManager);

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
    /**
     * @brief Queries whether the specified cell type is traversible by the player or not
     * @param[in] cellData Cell data
     * @return Returns true if the specified cell type is traversible. Returns false otherwise
     */
    bool IsTraversible(const CellData* cellData);

    /**
     * @brief Queries whether any of the keys in the specified list of keys 
     * is pressed or not in the current frame
     * @param[in] keys List of keys
     * @return Returns true if any of the keys in the specified key list is pressed
     * in the current frame
     */
    bool IsAnyKeyPressed(const std::vector<uint32_t>& keys);

    /**
     * @brief Resets the current level
     */
    void ResetCurrentLevel();

    /**
     * @brief Resets the current room
     */
    void ResetCurrentRoom();

    /**
     * @brief Checks if the specified room is completed or not
     * @param[in] roomData Room data
     * @return Returns true if the specified room is completed
     */
    bool IsRoomComplete(RoomData &roomData);
};
