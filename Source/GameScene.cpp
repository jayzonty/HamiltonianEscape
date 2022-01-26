#include "GameScene.hpp"

#include "Constants.hpp"
#include "LevelData.hpp"

#include <iostream>
#include <raylib.h>

#define CELL_SIZE 32.0f

/**
 * @brief Constructor
 */
GameScene::GameScene()
    : Scene()
    , m_levels()
    , m_currentLevelIndex(-1)
    , m_currentRoomIndex(-1)
    , m_moveUpKeys()
    , m_moveDownKeys()
    , m_moveLeftKeys()
    , m_moveRightKeys()
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
    m_levels.emplace_back();
    if (m_levels.back().LoadFromFile("Resources/Levels/level0.dat"))
    {
        std::cout << "Successfully loaded level 0!" << std::endl;
        m_currentLevelIndex = 0;
    }
    else
    {
        std::cerr << "Failed to load level 0!" << std::endl;
    }

    // --- Set up keybindings ---
    m_moveUpKeys.push_back(KEY_W);
    m_moveUpKeys.push_back(KEY_UP);

    m_moveDownKeys.push_back(KEY_S);
    m_moveDownKeys.push_back(KEY_DOWN);

    m_moveLeftKeys.push_back(KEY_A);
    m_moveLeftKeys.push_back(KEY_LEFT);

    m_moveRightKeys.push_back(KEY_D);
    m_moveRightKeys.push_back(KEY_RIGHT);

    ResetCurrentLevel();
}

/**
 * @brief Updates the scene
 * @param deltaTime Time elapsed since the previous frame
 */
void GameScene::Update(const float& deltaTime)
{
    if ((m_currentLevelIndex >= 0) && (m_currentRoomIndex >= 0))
    {
        LevelData &levelData = m_levels[m_currentLevelIndex];
        RoomData &roomData = levelData.rooms[m_currentRoomIndex];

        int32_t moveX = 0, moveY = 0;
        if (IsAnyKeyPressed(m_moveUpKeys))
        {
            moveY = -1;
        }
        else if (IsAnyKeyPressed(m_moveDownKeys))
        {
            moveY = 1;
        }

        if (IsAnyKeyPressed(m_moveLeftKeys))
        {
            moveX = -1;
        }

        else if (IsAnyKeyPressed(m_moveRightKeys))
        {
            moveX = 1;
        }

        if ((moveX != 0) || (moveY != 0))
        {
            int32_t currentPlayerX = m_playerPositionX;
            int32_t currentPlayerY = m_playerPositionY;
            int32_t newPlayerX = currentPlayerX + moveX;
            int32_t newPlayerY = currentPlayerY + moveY;
            if (roomData.cells.IsValidLocation(newPlayerX, newPlayerY)
                && IsTraversible(roomData.cells.Get(newPlayerX, newPlayerY)->type))
            {
                m_playerPositionX = newPlayerX;
                m_playerPositionY = newPlayerY;

                CellData *cellData = roomData.cells.Get(newPlayerX, newPlayerY);

                if ((currentPlayerX != newPlayerX)
                    || (currentPlayerY != newPlayerY))
                {
                    if (cellData->type == CellData::Type::Floor)
                    {
                        if (cellData->state == Constants::FLOOR_UNVISITED_STATE)
                        {
                            cellData->state = Constants::FLOOR_VISITED_STATE;

                            if (IsRoomComplete(roomData))
                            {
                                CellData *goalCell = roomData.cells.Get(roomData.goalX, roomData.goalY);
                                goalCell->state = Constants::GOAL_UNLOCKED_STATE;
                            }
                            else
                            {
                            }
                        }
                        else if( cellData->state == Constants::FLOOR_VISITED_STATE)
                        {
                            std::cout << "Illegal move!" << std::endl;
                        }
                    }
                    else if (cellData->type == CellData::Type::Goal)
                    {
                        if (cellData->state == Constants::GOAL_UNLOCKED_STATE)
                        {
                            int32_t numRooms = levelData.rooms.size();
                            if (m_currentRoomIndex + 1 < numRooms)
                            {
                                ++m_currentRoomIndex;
                                ResetCurrentRoom();
                            }
                            else
                            {
                                std::cout << "Change levels!" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Draws the scene
 */
void GameScene::Draw()
{
    BeginDrawing();
    ClearBackground(WHITE);

    if ((m_currentLevelIndex >= 0)
        && (m_currentRoomIndex >= 0))
    {
        LevelData &levelData = m_levels[m_currentLevelIndex];
        RoomData &roomData = levelData.rooms[m_currentRoomIndex];
        int32_t roomWidth = roomData.cells.GetWidth();
        int32_t roomHeight = roomData.cells.GetHeight();

        // Draw grid
        for (int32_t x = 0; x < roomWidth; ++x)
        {
            for (int32_t y = 0; y < roomHeight; ++y)
            {
                CellData *cellData = roomData.cells.Get(x, y);
                switch (cellData->type)
                {
                    case CellData::Type::Empty:
                        break;
                    case CellData::Type::Door:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BROWN);
                        break;
                    case CellData::Type::Floor:
                        break;
                    case CellData::Type::Goal:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
                        break;
                    case CellData::Type::Wall:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
                        break;
                }
            }
        }

        // Overlay a red rectangle on top of visited cells
        for (int32_t x = 0; x < roomWidth; ++x)
        {
            for (int32_t y = 0; y < roomHeight; ++y)
            {
                CellData *cellData = roomData.cells.Get(x, y);
                if ((cellData->type == CellData::Type::Floor)
                    && (cellData->state == Constants::FLOOR_VISITED_STATE)
                    && ((m_playerPositionX != x) || (m_playerPositionY != y)))
                {
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, ColorAlpha(RED, 0.75f));
                }
            }
        }

        // Draw player
        DrawCircle((m_playerPositionX + 0.5f) * CELL_SIZE, (m_playerPositionY + 0.5f) * CELL_SIZE, (CELL_SIZE - 6.0f) / 2.0f, BLUE);
    }
    
    EndDrawing();
}

/**
 * @brief Function called when the scene is about to end
 */
void GameScene::End()
{
}

/**
 * @brief Queries whether the specified cell type is traversible by the player or not
 * @param[in] cellType Cell type
 * @return Returns true if the specified cell type is traversible. Returns false otherwise
 */
bool GameScene::IsTraversible(CellData::Type cellType)
{
    if (cellType == CellData::Type::Wall)
    {
        return false;
    }

    return true;
}

/**
 * @brief Queries whether any of the keys in the specified list of keys 
 * is pressed or not in the current frame
 * @param[in] keys List of keys
 * @return Returns true if any of the keys in the specified key list is pressed
 * in the current frame
 */
bool GameScene::IsAnyKeyPressed(const std::vector<uint32_t>& keys)
{
    for (uint32_t key : keys)
    {
        if (IsKeyPressed(key))
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Resets the current level
 */
void GameScene::ResetCurrentLevel()
{
    m_currentRoomIndex = 0;
    ResetCurrentRoom();
}

/**
 * @brief Resets the current room
 */
void GameScene::ResetCurrentRoom()
{
    if ((m_currentLevelIndex == -1) || (m_currentRoomIndex == -1))
    {
        return;
    }

    LevelData &levelData = m_levels[m_currentLevelIndex];
    RoomData &roomData = levelData.rooms[m_currentRoomIndex];
    m_playerPositionX = roomData.playerStartX;
    m_playerPositionY = roomData.playerStartY;

    int32_t roomWidth = roomData.cells.GetWidth();
    int32_t roomHeight = roomData.cells.GetHeight();
    for (int32_t x = 0; x < roomWidth; ++x)
    {
        for (int32_t y = 0; y < roomHeight; ++y)
        {
            CellData *cellData = roomData.cells.Get(x, y);
            cellData->state = 0;
        }
    }

    CellData *playerCell = roomData.cells.Get(m_playerPositionX, m_playerPositionY);
    if ((playerCell != nullptr)
        && (playerCell->type == CellData::Type::Floor))
    {
        playerCell->state = Constants::FLOOR_VISITED_STATE;
    }

    CellData *goalCell = roomData.cells.Get(roomData.goalX, roomData.goalY);
    if (goalCell != nullptr)
    {
        goalCell->state = Constants::GOAL_LOCKED_STATE;
    }
}

/**
 * @brief Checks if the specified room is completed or not
 * @param[in] roomData Room data
 * @return Returns true if the specified room is completed
 */
bool GameScene::IsRoomComplete(RoomData &roomData)
{
    int32_t roomWidth = roomData.cells.GetWidth();
    int32_t roomHeight = roomData.cells.GetHeight();
    for (int32_t x = 0; x < roomWidth; ++x)
    {
        for (int32_t y = 0; y < roomHeight; ++y)
        {
            CellData *cellData = roomData.cells.Get(x, y);
            if (cellData->type == CellData::Type::Floor)
            {
                if (cellData->state == Constants::FLOOR_UNVISITED_STATE)
                {
                    return false;
                }
            }
        }
    }

    return true;
}
