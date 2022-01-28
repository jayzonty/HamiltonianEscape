#include "TitleScene.hpp"

#include "Constants.hpp"

#include <raylib.h>

#include <string>

/**
 * @brief Constructor
 */
TitleScene::TitleScene(SceneManager *sceneManager)
    : Scene(sceneManager)
    , m_startGameButtonBounds()
{
}

/**
 * @brief Destructor
 */
TitleScene::~TitleScene()
{
}

/**
 * @brief Function called when the scene is about to begin
 */
void TitleScene::Begin()
{
    m_startGameButtonBounds.x = 325.0f;
    m_startGameButtonBounds.y = 275.0f;
    m_startGameButtonBounds.width = 150.0f;
    m_startGameButtonBounds.height = 50.0f;
}

/**
 * @brief Updates the scene
 * @param deltaTime Time elapsed since the previous frame
 */
void TitleScene::Update(const float& deltaTime)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, m_startGameButtonBounds))
        {
            GetSceneManager()->SwitchToScene(Constants::GAME_SCENE_ID);
        }
    }
}

/**
 * @brief Draws the scene
 */
void TitleScene::Draw()
{
    BeginDrawing();
    ClearBackground(BLUE);

    // Draw title
    const int32_t titleFontSize = 60;
    const std::string titleText = "Hamiltonian Escape";
    const int32_t titleTextWidth = MeasureText(titleText.c_str(), titleFontSize);
    const int32_t titleTextX = (GetScreenWidth() - titleTextWidth) / 2.0f;
    const int32_t titleTextY = 100;
    const int32_t dropShadowOffsetX = 5, dropShadowOffsetY = 5;
    // Drop shadow
    DrawText(
        titleText.c_str(),
        (GetScreenWidth() - titleTextWidth) / 2.0f + dropShadowOffsetX,
        titleTextY + dropShadowOffsetY,
        titleFontSize,
        BLACK
    );
    // Text
    DrawText(
        titleText.c_str(),
        titleTextX,
        titleTextY,
        titleFontSize,
        WHITE
    );

    DrawRectangleRec(m_startGameButtonBounds, BLACK);
    float fontSize = 32.0f;
    float textWidth = MeasureText("Start", fontSize);
    float paddingLeft = (m_startGameButtonBounds.width - textWidth) / 2.0f;
    float paddingTop = (m_startGameButtonBounds.height - fontSize) / 2.0f;
    DrawText(
        "Start", 
        m_startGameButtonBounds.x + paddingLeft,
        m_startGameButtonBounds.y + paddingTop,
        fontSize,
        WHITE
    );

    EndDrawing();
}

/**
 * @brief Function called when the scene is about to end
 */
void TitleScene::End()
{
}
