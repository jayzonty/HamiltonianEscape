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
    m_startGameButtonBounds.width = 150.0f;
    m_startGameButtonBounds.height = 50.0f;
    m_startGameButtonBounds.x = (GetScreenWidth() - m_startGameButtonBounds.width) / 2;
    m_startGameButtonBounds.y = GetScreenHeight() / 2.0f + 50;
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
    const int32_t titleTextX = (GetScreenWidth() - titleTextWidth) / 2;
    const int32_t titleTextY = (GetScreenHeight() - titleFontSize) / 2 - 50;
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

    const int32_t buttonOutlineThickness = 4;
    DrawRectangleRec(m_startGameButtonBounds, BLACK);
    DrawRectangle(
        m_startGameButtonBounds.x + buttonOutlineThickness,
        m_startGameButtonBounds.y + buttonOutlineThickness,
        m_startGameButtonBounds.width - buttonOutlineThickness * 2,
        m_startGameButtonBounds.height - buttonOutlineThickness * 2,
        WHITE
    );
    float fontSize = 32.0f;
    float textWidth = MeasureText("Start", fontSize);
    float paddingLeft = (m_startGameButtonBounds.width - textWidth) / 2.0f;
    float paddingTop = (m_startGameButtonBounds.height - fontSize) / 2.0f;
    DrawText(
        "Start", 
        m_startGameButtonBounds.x + paddingLeft,
        m_startGameButtonBounds.y + paddingTop,
        fontSize,
        BLACK
    );

    EndDrawing();
}

/**
 * @brief Function called when the scene is about to end
 */
void TitleScene::End()
{
}
