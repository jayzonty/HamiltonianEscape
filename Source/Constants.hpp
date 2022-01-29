
#include <cstdint>
#include <string>

namespace Constants
{
constexpr uint32_t TITLE_SCENE_ID = 0;
constexpr uint32_t GAME_SCENE_ID = 1;

constexpr int32_t GOAL_LOCKED_STATE = 0;
constexpr int32_t GOAL_UNLOCKED_STATE = 1;

constexpr int32_t SWITCH_UNPRESSED_STATE = 0;
constexpr int32_t SWITCH_PRESSED_STATE = 1;

constexpr int32_t DOOR_LOCKED_STATE = 0;
constexpr int32_t DOOR_UNLOCKED_STATE = 1;

const std::string SLIDE_UP_TEXTURE_NAME = "slide_up";
const std::string SLIDE_DOWN_TEXTURE_NAME = "slide_down";
const std::string SLIDE_LEFT_TEXTURE_NAME = "slide_left";
const std::string SLIDE_RIGHT_TEXTURE_NAME = "slide_right";
}
