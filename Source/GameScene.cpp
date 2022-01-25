#include "GameScene.hpp"

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
    if (m_currentLevelIndex >= 0)
    {
        LevelData &levelData = m_levels[m_currentLevelIndex];

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
            int32_t currentPlayerX = levelData.playerPositionX;
            int32_t currentPlayerY = levelData.playerPositionY;
            int32_t newPlayerX = currentPlayerX + moveX;
            int32_t newPlayerY = currentPlayerY + moveY;
            if (levelData.cells.IsValidLocation(newPlayerX, newPlayerY)
                && IsTraversible(levelData.cells.Get(newPlayerX, newPlayerY)->type))
            {
                levelData.playerPositionX = newPlayerX;
                levelData.playerPositionY = newPlayerY;

                int32_t currentRoomIndex = levelData.GetRoomIndexAtPosition(
                    currentPlayerX, currentPlayerY);
                int32_t newRoomIndex = levelData.GetRoomIndexAtPosition(
                    newPlayerX, newPlayerY);

                if ((newRoomIndex != -1)
                    && (currentRoomIndex != newRoomIndex))
                {
                    if (levelData.rooms[newRoomIndex].isVisited)
                    {
                        std::cout << "Illegal move! Revisited a room!" << std::endl;
                    }
                    else
                    {
                        levelData.rooms[newRoomIndex].isVisited = true;
                    }
                }

                CellData *cellData = levelData.cells.Get(newPlayerX, newPlayerY);
                if (cellData->type == CellData::Type::Goal)
                {
                    bool isLevelComplete = true;

                    int32_t numRooms = levelData.rooms.size();
                    for (int32_t i = 0; i < numRooms; ++i)
                    {
                        if (!levelData.rooms[i].isVisited)
                        {
                            isLevelComplete = false;
                            break;
                        }
                    }

                    if (isLevelComplete)
                    {
                        std::cout << "Level complete!" << std::endl;
                    }
                    else
                    {
                        std::cout << "There are still unvisited rooms" << std::endl;
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

    if (m_currentLevelIndex >= 0)
    {
        LevelData &levelData = m_levels[m_currentLevelIndex];
        int32_t levelWidth = levelData.cells.GetWidth();
        int32_t levelHeight = levelData.cells.GetHeight();

        // Draw grid
        for (int32_t x = 0; x < levelWidth; ++x)
        {
            for (int32_t y = 0; y < levelHeight; ++y)
            {
                CellData *cellData = levelData.cells.Get(x, y);
                switch (cellData->type)
                {
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

        int32_t playerCurrentRoomIndex = levelData.GetRoomIndexAtPosition(levelData.playerPositionX, levelData.playerPositionY);
        // Overlay a red rectangle on the cells in visited rooms
        int32_t numRooms = levelData.rooms.size();
        for (int32_t i = 0; i < numRooms; ++i)
        {
            if ((i != playerCurrentRoomIndex)
                && (levelData.rooms[i].isVisited))
            {
                int32_t upperLeftX = levelData.rooms[i].upperLeftX;
                int32_t upperLeftY = levelData.rooms[i].upperLeftY;
                int32_t lowerRightX = upperLeftX + levelData.rooms[i].width;
                int32_t lowerRightY = upperLeftY + levelData.rooms[i].height;

                for (int32_t x = upperLeftX; x < lowerRightX; ++x)
                {
                    for (int32_t y = upperLeftY; y < lowerRightY; ++y)
                    {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, ColorAlpha(RED, 0.75f));
                    }
                }
            }
        }

        // Draw player
        int32_t playerX = levelData.playerPositionX;
        int32_t playerY = levelData.playerPositionY;
        DrawCircle((playerX + 0.5f) * CELL_SIZE, (playerY + 0.5f) * CELL_SIZE, (CELL_SIZE - 6.0f) / 2.0f, BLUE);
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

void GameScene::ResetCurrentLevel()
{
    if (m_currentLevelIndex == -1)
    {
        return;
    }

    LevelData &levelData = m_levels[m_currentLevelIndex];
    levelData.playerPositionX = levelData.playerStartX;
    levelData.playerPositionY = levelData.playerStartY;

    int32_t levelWidth = levelData.cells.GetWidth();
    int32_t levelHeight = levelData.cells.GetHeight();
    for (int32_t x = 0; x < levelWidth; ++x)
    {
        for (int32_t y = 0; y < levelHeight; ++y)
        {
            CellData *cellData = levelData.cells.Get(x, y);
            cellData->state = 0;
        }
    }

    int32_t numRooms = levelData.rooms.size();
    for (int32_t i = 0; i < numRooms; ++i)
    {
        levelData.rooms[i].isVisited = false;
    }

    int32_t playerRoomIndex = levelData.GetRoomIndexAtPosition(levelData.playerPositionX, levelData.playerPositionY);
    if (playerRoomIndex != -1)
    {
        levelData.rooms[playerRoomIndex].isVisited = true;
    }
}
