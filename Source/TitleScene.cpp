#include "TitleScene.hpp"

#include "Constants.hpp"

#include <raylib.h>

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
