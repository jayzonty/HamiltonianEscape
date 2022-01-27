#include "GameScene.hpp"

#include "Constants.hpp"
#include "LevelData.hpp"

#include <array>
#include <cctype>
#include <iostream>
#include <raylib.h>
#include <string>

#define CELL_SIZE 32.0f
#define SWITCH_RADIUS 12.0f
#define SWITCH_LABEL_FONT_SIZE 24 

/**
 * @brief Constructor
 */
GameScene::GameScene(SceneManager *sceneManager)
    : Scene(sceneManager)
    , m_levels()
    , m_currentLevelIndex(-1)
    , m_currentRoomIndex(-1)
    , m_playerPositionX(0)
    , m_playerPositionY(0)
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
    m_levels.clear();

    std::array<std::string, 3> levelFilePaths =
    {
        "Resources/Levels/level0.dat",
        "Resources/Levels/level1.dat",
        "Resources/Levels/level2.dat",
    };
    for (size_t i = 0; i < levelFilePaths.size(); ++i)
    {
        m_levels.emplace_back();
        if (m_levels.back().LoadFromFile(levelFilePaths[i]))
        {
            std::cout << "Successfully loaded level " << i << "!" << std::endl;
        }
        else
        {
            std::cerr << "Failed to load level " << i << "!" << std::endl;
        }
    }
    m_currentLevelIndex = 0;

    // --- Set up keybindings ---
    m_moveUpKeys.push_back(KEY_W);
    m_moveUpKeys.push_back(KEY_UP);

    m_moveDownKeys.push_back(KEY_S);
    m_moveDownKeys.push_back(KEY_DOWN);

    m_moveLeftKeys.push_back(KEY_A);
    m_moveLeftKeys.push_back(KEY_LEFT);

    m_moveRightKeys.push_back(KEY_D);
    m_moveRightKeys.push_back(KEY_RIGHT);

    m_resetRoomKeys.push_back(KEY_R);

    m_resetButtonBounds.width = 160.0f;
    m_resetButtonBounds.height = 50.0f;
    m_resetButtonBounds.x = 50.0f;
    m_resetButtonBounds.y = 600.0f - m_resetButtonBounds.height - 10.0f;

    ResetCurrentLevel();
}

/**
 * @brief Updates the scene
 * @param deltaTime Time elapsed since the previous frame
 */
void GameScene::Update(const float& deltaTime)
{
    Vector2 mousePosition = GetMousePosition();
    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, m_resetButtonBounds))
        || IsAnyKeyPressed(m_resetRoomKeys))

    {
        ResetCurrentRoom();
        return;
    }

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
                && IsTraversible(roomData.cells.Get(newPlayerX, newPlayerY)))
            {
                m_playerPositionX = newPlayerX;
                m_playerPositionY = newPlayerY;

                CellData *cellData = roomData.cells.Get(newPlayerX, newPlayerY);
                cellData->isVisited = true;

                if ((currentPlayerX != newPlayerX)
                    || (currentPlayerY != newPlayerY))
                {
                    if (cellData->type == CellData::Type::Floor)
                    {
                    }
                    else if (cellData->type == CellData::Type::Switch)
                    {
                        int32_t switchId = roomData.GetSwitchIdFromSwitchPosition(newPlayerX, newPlayerY);
                        if (switchId != -1)
                        {
                            cellData->state = Constants::SWITCH_PRESSED_STATE;

                            SwitchDoorMapping &mapping = roomData.switchDoorMappings[switchId];
                            CellData *doorCell = roomData.cells.Get(mapping.doorX, mapping.doorY);
                            doorCell->state = Constants::DOOR_UNLOCKED_STATE;
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
                                int32_t numLevels = m_levels.size();
                                if (m_currentLevelIndex + 1 < numLevels)
                                {
                                    ++m_currentLevelIndex;
                                    ResetCurrentLevel();
                                }
                                else
                                {
                                    std::cout << "Finished all levels!" << std::endl;
                                    GetSceneManager()->SwitchToScene(Constants::TITLE_SCENE_ID);
                                }
                            }
                        }
                    }

                    if (IsRoomComplete(roomData))
                    {
                        CellData *goalCell = roomData.cells.Get(roomData.goalX, roomData.goalY);
                        goalCell->state = Constants::GOAL_UNLOCKED_STATE;
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
                if (cellData->type == CellData::Type::Empty)
                {
                }
                else if (cellData->type == CellData::Type::Switch)
                {
                    if (cellData->state == Constants::SWITCH_UNPRESSED_STATE)
                    {
                        DrawCircle((x + 0.5f) * CELL_SIZE, (y + 0.5f) * CELL_SIZE, SWITCH_RADIUS, RED);

                        int32_t switchId = roomData.GetSwitchIdFromSwitchPosition(x, y);
                        if (switchId != -1)
                        {
                            char c = static_cast<char>(switchId);
                            c = std::toupper(c);

                            std::string text;
                            text.push_back(c);

                            Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), SWITCH_LABEL_FONT_SIZE, 0.0f);
                            float paddingLeft = (CELL_SIZE - textSize.x) / 2;
                            float paddingTop = (CELL_SIZE - textSize.y) / 2;
                            Vector2 textPosition { x * CELL_SIZE + paddingLeft, y * CELL_SIZE + paddingTop };
                            DrawTextEx(GetFontDefault(), text.c_str(), textPosition, SWITCH_LABEL_FONT_SIZE, 0.0f, BLACK);
                        }
                    }
                }
                else if (cellData->type == CellData::Type::Door)
                {
                    if (cellData->state == Constants::DOOR_LOCKED_STATE)
                    {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BROWN);

                        int32_t switchId = roomData.GetSwitchIdFromDoorPosition(x, y);
                        if (switchId != -1)
                        {
                            char c = static_cast<char>(switchId);
                            c = std::toupper(c);

                            std::string text;
                            text.push_back(c);

                            Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), SWITCH_LABEL_FONT_SIZE, 0.0f);
                            float paddingLeft = (CELL_SIZE - textSize.x) / 2;
                            float paddingTop = (CELL_SIZE - textSize.y) / 2;
                            Vector2 textPosition { x * CELL_SIZE + paddingLeft, y * CELL_SIZE + paddingTop };
                            DrawTextEx(GetFontDefault(), text.c_str(), textPosition, SWITCH_LABEL_FONT_SIZE, 0.0f, BLACK);
                        }
                    }
                }
                else if (cellData->type == CellData::Type::Floor)
                {
                }
                else if (cellData->type == CellData::Type::Goal)
                {
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
                }
                else if (cellData->type == CellData::Type::Wall)
                {
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
                }
            }
        }

        // Overlay a red rectangle on top of visited cells
        for (int32_t x = 0; x < roomWidth; ++x)
        {
            for (int32_t y = 0; y < roomHeight; ++y)
            {
                CellData *cellData = roomData.cells.Get(x, y);
                if (cellData->isVisited
                    && ((m_playerPositionX != x) || (m_playerPositionY != y)))
                {
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, ColorAlpha(RED, 0.75f));
                }
            }
        }

        // Draw player
        DrawCircle((m_playerPositionX + 0.5f) * CELL_SIZE, (m_playerPositionY + 0.5f) * CELL_SIZE, (CELL_SIZE - 6.0f) / 2.0f, BLUE);
    }

    // Draw reset button
    std::string resetText = "Reset (R)";
    float resetTextFontSize = 32.0f;
    DrawRectangleRec(m_resetButtonBounds, BLACK);
    int32_t resetTextWidth = MeasureText(
        resetText.c_str(),
        resetTextFontSize);
    float resetTextPaddingLeft = (m_resetButtonBounds.width - resetTextWidth) / 2.0f;
    float resetTextPaddingTop = (m_resetButtonBounds.height - resetTextFontSize) / 2.0f;
    DrawText(
        resetText.c_str(),
        m_resetButtonBounds.x + resetTextPaddingLeft,
        m_resetButtonBounds.y + resetTextPaddingTop,
        resetTextFontSize,
        WHITE);
    
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
 * @param[in] cellData Cell data
 * @return Returns true if the specified cell type is traversible. Returns false otherwise
 */
bool GameScene::IsTraversible(const CellData* cellData)
{
    if (cellData == nullptr)
    {
        return false;
    }
    if (cellData->type == CellData::Type::Wall)
    {
        return false;
    }
    if (cellData->isVisited)
    {
        return false;
    }
    if ((cellData->type == CellData::Type::Door)
        && (cellData->state == Constants::DOOR_LOCKED_STATE))
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
            cellData->isVisited = false;
        }
    }

    CellData *playerCell = roomData.cells.Get(m_playerPositionX, m_playerPositionY);
    if ((playerCell != nullptr)
        && (playerCell->type == CellData::Type::Floor))
    {
        playerCell->isVisited = true;
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
                if (!cellData->isVisited)
                {
                    return false;
                }
            }
        }
    }

    return true;
}
