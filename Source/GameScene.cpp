#include "GameScene.hpp"

#include "Constants.hpp"
#include "LevelData.hpp"

#include <array>
#include <cctype>
#include <iostream>
#include <raylib.h>
#include <string>

#define CELL_SIZE 40.0f
#define SWITCH_RADIUS 12.0f
#define SWITCH_LABEL_FONT_SIZE 24 

#define START_LEVEL_TIMER_DURATION 2.0f
#define START_LEVEL_FADE_IN_DURATION 1.0f
#define END_LEVEL_FADE_OUT_DURATION 1.0f

#define INSTRUCTIONS_TEXT_FONT_SIZE 24

#define SLIDE_DURATION 0.1f

/**
 * @brief Constructor
 */
GameScene::GameScene(SceneManager *sceneManager)
    : Scene(sceneManager)
    , m_currentState(State::Play)
    , m_levels()
    , m_currentLevelIndex(-1)
    , m_currentRoomIndex(-1)
    , m_playerPositionX(0)
    , m_playerPositionY(0)
    , m_playerVisualScale(1.0f)
    , m_startLevelTimer(0.0f)
    , m_startLevelFadeInTimer(0.0f)
    , m_endLevelFadeOutTimer(0.0f)
    , m_resetButtonBounds()
    , m_backToTitleButton()
    , m_slideDirectionX(-1)
    , m_slideDirectionY(-1)
    , m_slideTimer(0.0f)
    , m_isSliding(false)
    , m_moveUpKeys()
    , m_moveDownKeys()
    , m_moveLeftKeys()
    , m_moveRightKeys()
    , m_backgroundMusic()
    , m_textures()
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

    std::array<std::string, 4> levelFilePaths =
    {
        "Resources/Levels/level1.dat",
        "Resources/Levels/level2.dat",
        "Resources/Levels/level3.dat",
        "Resources/Levels/level4.dat",
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

    m_resetButtonBounds.width = 140.0f;
    m_resetButtonBounds.height = 50.0f;
    m_resetButtonBounds.x = GetScreenWidth() - m_resetButtonBounds.width - 10.0f;
    m_resetButtonBounds.y = GetScreenHeight() - m_resetButtonBounds.height - 10.0f;

    m_backToTitleButton.width = 200.0f;
    m_backToTitleButton.height = 50.0f;
    m_backToTitleButton.x = (GetScreenWidth() - m_backToTitleButton.width) / 2.0f;
    m_backToTitleButton.y = (GetScreenHeight() - m_backToTitleButton.height) / 2.0f;

    ResetCurrentLevel();

    m_backgroundMusic = LoadMusicStream("Resources/Audio/bgm.wav");
    m_backgroundMusic.looping = true;

    // HACK: To make the audio loop more seamless.
    // I figure it's because raylib immediately stops the song
    // upon reaching the end of the buffer before the data can be 
    // sent to the speakers. For now, I just added a big number to the frame count
    m_backgroundMusic.frameCount += 500; 

    PlayMusicStream(m_backgroundMusic);

    m_currentState = State::StartLevel;
    m_startLevelTimer = START_LEVEL_TIMER_DURATION;
    m_startLevelFadeInTimer = START_LEVEL_FADE_IN_DURATION;
    m_playerVisualScale = 0.0f;

    m_textures[Constants::SLIDE_UP_TEXTURE_NAME] = LoadTexture("Resources/Sprites/slide_up.png");
    m_textures[Constants::SLIDE_DOWN_TEXTURE_NAME] = LoadTexture("Resources/Sprites/slide_down.png");
    m_textures[Constants::SLIDE_LEFT_TEXTURE_NAME] = LoadTexture("Resources/Sprites/slide_left.png");
    m_textures[Constants::SLIDE_RIGHT_TEXTURE_NAME] = LoadTexture("Resources/Sprites/slide_right.png");
}

/**
 * @brief Updates the scene
 * @param deltaTime Time elapsed since the previous frame
 */
void GameScene::Update(const float& deltaTime)
{
    UpdateMusicStream(m_backgroundMusic);

    if (m_currentState == State::Play)
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
            if (m_isSliding)
            {
                m_slideTimer -= deltaTime;
                if (m_slideTimer <= 0.0f)
                {
                    m_slideTimer = SLIDE_DURATION;
                    Move(m_slideDirectionX, m_slideDirectionY);

                    LevelData &levelData = m_levels[m_currentLevelIndex];
                    RoomData &roomData = levelData.rooms[m_currentRoomIndex];

                    int32_t nextPlayerX = m_playerPositionX + m_slideDirectionX;
                    int32_t nextPlayerY = m_playerPositionY + m_slideDirectionY;

                    CellData *currentCellData = roomData.cells.Get(m_playerPositionX, m_playerPositionY);
                    CellData *nextCellData = roomData.cells.Get(nextPlayerX, nextPlayerY);
                    if (!IsTraversible(nextCellData) 
                        || (currentCellData->type != CellData::Type::Ice))
                    {
                        m_isSliding = false;
                    }
                }
            }
            else
            {
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
                    Move(moveX, moveY);
                }
            }
        }
    }
    else if (m_currentState == State::StartRoom)
    {
        const float playerVisualScaleAnimSpeed = 1.0f; // Scale per second
        m_playerVisualScale += playerVisualScaleAnimSpeed * deltaTime;
        if (m_playerVisualScale >= 1.0f)
        {
            m_playerVisualScale = 1.0f;

            m_currentState = State::Play;
        }
    }
    else if (m_currentState == State::EndRoom)
    {
        const float playerVisualScaleAnimSpeed = 1.0f; // Scale per second
        m_playerVisualScale -= playerVisualScaleAnimSpeed * deltaTime;

        if (m_playerVisualScale <= 0.0f)
        {
            LevelData &levelData = m_levels[m_currentLevelIndex];

            int32_t numRooms = levelData.rooms.size();
            if (m_currentRoomIndex + 1 < numRooms)
            {
                ++m_currentRoomIndex;
                ResetCurrentRoom();

                m_currentState = State::StartRoom;
            }
            else
            {
                m_endLevelFadeOutTimer = END_LEVEL_FADE_OUT_DURATION;
                m_currentState = State::EndLevel;
            }
        }
    }
    else if (m_currentState == State::StartLevel)
    {
        if (m_startLevelTimer <= 0.0f)
        {
            m_startLevelFadeInTimer -= deltaTime;
            if (m_startLevelFadeInTimer <= 0.0f)
            {
                m_currentState = State::StartRoom;
            }
        }
        else
        {
            m_startLevelTimer -= deltaTime;
        }
    }
    else if (m_currentState == State::EndLevel)
    {
        m_endLevelFadeOutTimer -= deltaTime;
        if (m_endLevelFadeOutTimer <= 0.0f)
        {
            int32_t numLevels = m_levels.size();
            if (m_currentLevelIndex + 1 < numLevels)
            {
                ++m_currentLevelIndex;
                ResetCurrentLevel();

                m_currentState = State::StartLevel;
                m_startLevelTimer = START_LEVEL_TIMER_DURATION;
                m_startLevelFadeInTimer = START_LEVEL_FADE_IN_DURATION;
            }
            else
            {
                m_currentState = State::GameEnd;
            }
        }
    }
    else if (m_currentState == State::GameEnd)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(GetMousePosition(), m_backToTitleButton))
            {
                GetSceneManager()->SwitchToScene(Constants::TITLE_SCENE_ID);
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
        && (m_currentRoomIndex >= 0)
        && (m_currentState != State::GameEnd))
    {
        LevelData &levelData = m_levels[m_currentLevelIndex];
        RoomData &roomData = levelData.rooms[m_currentRoomIndex];
        int32_t roomWidth = roomData.cells.GetWidth();
        int32_t roomHeight = roomData.cells.GetHeight();

        Vector2 offset =
        {
            (GetScreenWidth() - roomWidth * CELL_SIZE) / 2.0f, 
            (GetScreenHeight() - roomHeight * CELL_SIZE) / 2.0f
        };

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
                        DrawCircle(offset.x + (x + 0.5f) * CELL_SIZE, offset.y + (y + 0.5f) * CELL_SIZE, SWITCH_RADIUS, RED);

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
                            Vector2 textPosition { offset.x + x * CELL_SIZE + paddingLeft, offset.y + y * CELL_SIZE + paddingTop };
                            DrawTextEx(GetFontDefault(), text.c_str(), textPosition, SWITCH_LABEL_FONT_SIZE, 0.0f, BLACK);
                        }
                    }
                }
                else if (cellData->type == CellData::Type::Door)
                {
                    if (cellData->state == Constants::DOOR_LOCKED_STATE)
                    {
                        DrawRectangle(offset.x + x * CELL_SIZE, offset.y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BROWN);

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
                            Vector2 textPosition { offset.x + x * CELL_SIZE + paddingLeft, offset.y + y * CELL_SIZE + paddingTop };
                            DrawTextEx(GetFontDefault(), text.c_str(), textPosition, SWITCH_LABEL_FONT_SIZE, 0.0f, BLACK);
                        }
                    }
                }
                else if (cellData->type == CellData::Type::Floor)
                {
                }
                else if (cellData->type == CellData::Type::Goal)
                {
                    DrawRectangle(offset.x + x * CELL_SIZE, offset.y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);

                    std::string text = "GOAL";
                    float fontSize = 14.0f;
                    int32_t textWidth = MeasureText(text.c_str(), fontSize);
                    float paddingLeft = (CELL_SIZE - textWidth) / 2.0f;
                    float paddingTop = (CELL_SIZE - fontSize) / 2.0f;
                    DrawText(
                        text.c_str(), 
                        offset.x + paddingLeft + x * CELL_SIZE,
                        offset.y + paddingTop + y * CELL_SIZE,
                        fontSize,
                        BLACK
                    );
                }
                else if (cellData->type == CellData::Type::Wall)
                {
                    DrawRectangle(offset.x + x * CELL_SIZE, offset.y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
                }
                else if (cellData->type == CellData::Type::Ice)
                {
                    /*DrawTextureEx(
                        m_textures[Constants::SLIDE_UP_TEXTURE_NAME],
                        { offset.x + x * CELL_SIZE, offset.y + y * CELL_SIZE },
                        0.0f,
                        CELL_SIZE * 1.0f / m_textures[Constants::SLIDE_UP_TEXTURE_NAME].width,
                        WHITE
                    );*/
                    DrawRectangle(
                        offset.x + x * CELL_SIZE,
                        offset.y + y * CELL_SIZE,
                        CELL_SIZE,
                        CELL_SIZE,
                        GetColor(0xA5F2F3)
                    );
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
                    DrawRectangle(offset.x + x * CELL_SIZE, offset.y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, ColorAlpha(RED, 0.75f));
                }
            }
        }

        // Draw player
        float playerRadius = (CELL_SIZE - 6.0f) / 2.0f * m_playerVisualScale;
        DrawCircle(offset.x + (m_playerPositionX + 0.5f) * CELL_SIZE, offset.y + (m_playerPositionY + 0.5f) * CELL_SIZE, playerRadius, BLUE);

        // Draw reset button
        std::string resetText = "Reset (R)";
        float resetTextFontSize = 24.0f;
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

        // Draw instructions text
        std::string instructionsText = "W/A/S/D/Arrow Keys - Move";
        DrawText(
            instructionsText.c_str(), 
            10, 
            GetScreenHeight() - INSTRUCTIONS_TEXT_FONT_SIZE - 10,
            INSTRUCTIONS_TEXT_FONT_SIZE, 
            BLACK
        );
    }

    // Draw level start stuff
    if ((m_currentState == State::StartLevel) || (m_currentState == State::EndLevel))
    {
        float alpha = m_startLevelFadeInTimer / START_LEVEL_FADE_IN_DURATION;
        if (m_currentState == State::EndLevel)
        {
            alpha = (END_LEVEL_FADE_OUT_DURATION - m_endLevelFadeOutTimer) / END_LEVEL_FADE_OUT_DURATION;
        }

        DrawRectangle(0, 0, 800, 600, ColorAlpha(WHITE, alpha));

        if (m_currentState == State::StartLevel)
        {
            float fontSize = 48.0f;
            float spacing = 1.0f;
            std::string text = "Level " + std::to_string(m_currentLevelIndex + 1);

            Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, spacing);
            DrawTextPro(
                GetFontDefault(), 
                text.c_str(), 
                { 400.0f, 300.0f }, 
                { textSize.x / 2.0f, textSize.y / 2.0f },
                0.0f,
                fontSize, 
                spacing, 
                ColorAlpha(BLACK, alpha)
            );
        }
    }

    if (m_currentState == State::GameEnd)
    {
        const int32_t gameEndTextFontSize = 36;
        const std::string text = "You have completed all levels!";
        int32_t textWidth = MeasureText(text.c_str(), gameEndTextFontSize);
        DrawText(
            text.c_str(),
            (GetScreenWidth() - textWidth) / 2.0f,
            m_backToTitleButton.y - gameEndTextFontSize - 20,
            gameEndTextFontSize,
            BLACK
        );

        int32_t buttonTextFontSize = 24;
        int32_t buttonOutlineThickness = 4;
        DrawRectangleRec(m_backToTitleButton, BLACK);
        DrawRectangle(
            m_backToTitleButton.x + buttonOutlineThickness,
            m_backToTitleButton.y + buttonOutlineThickness,
            m_backToTitleButton.width - buttonOutlineThickness * 2,
            m_backToTitleButton.height - buttonOutlineThickness * 2,
            WHITE
        );
        const std::string buttonText = "Back to Title";
        textWidth = MeasureText(buttonText.c_str(), buttonTextFontSize);
        float paddingLeft = (m_backToTitleButton.width - textWidth) / 2.0f;
        float paddingTop = (m_backToTitleButton.height - buttonTextFontSize) / 2.0f;
        DrawText(
            buttonText.c_str(),
            m_backToTitleButton.x + paddingLeft,
            m_backToTitleButton.y + paddingTop,
            buttonTextFontSize,
            BLACK
        );
    }
    
    EndDrawing();
}

/**
 * @brief Function called when the scene is about to end
 */
void GameScene::End()
{
    // Unload all textures
    for (auto &it : m_textures)
    {
        UnloadTexture(it.second);
    }
    m_textures.clear();

    // Unload background music
    UnloadMusicStream(m_backgroundMusic);
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
 * @brief Moves the player by the specified move amount within the grid
 * @param[in] moveX Move amount in the x-axis
 * @param[in] moveY Move amount in the y-axis
 */
bool GameScene::Move(const int32_t& moveX, const int32_t& moveY)
{
    LevelData &levelData = m_levels[m_currentLevelIndex];
    RoomData &roomData = levelData.rooms[m_currentRoomIndex];

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
        if (cellData->type != CellData::Type::Ice)
        {
            cellData->isVisited = true;
        }

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
                    m_currentState = State::EndRoom;
                }
            }
            else if (cellData->type == CellData::Type::Ice)
            {
                m_isSliding = true;
                m_slideDirectionX = moveX;
                m_slideDirectionY = moveY;
                m_slideTimer = SLIDE_DURATION;
            }

            if (IsRoomComplete(roomData))
            {
                CellData *goalCell = roomData.cells.Get(roomData.goalX, roomData.goalY);
                goalCell->state = Constants::GOAL_UNLOCKED_STATE;
            }
        }

        return true;
    }

    return false;
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
    m_isSliding = false;

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
