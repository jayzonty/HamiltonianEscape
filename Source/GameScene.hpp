#pragma once

#include "LevelData.hpp"
#include "Scene.hpp"

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
    /**
     * @brief Queries whether the specified cell type is traversible by the player or not
     * @param[in] cellType Cell type
     * @return Returns true if the specified cell type is traversible. Returns false otherwise
     */
    bool IsTraversible(CellData::Type cellType);

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
