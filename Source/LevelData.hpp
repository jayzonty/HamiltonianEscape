#pragma once

#include "Grid.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

/**
 * Struct containing data about the mapping between
 * a switch and a door
 */
struct SwitchDoorMapping
{
    /**
     * Switch ID
     */
    int32_t switchId;

    /**
     * X-position of the switch
     */
    int32_t switchX;

    /**
     * Y-position of the switch
     */
    int32_t switchY;

    /**
     * X-position of the door
     */
    int32_t doorX;

    /**
     * Y-position of the door
     */
    int32_t doorY;
};

/**
 * Struct containing data about a cell in the level data
 */
struct CellData
{
    /**
     * Cell type enum
     */
    enum class Type
    {
        Empty,
        Floor,
        Switch,
        Door,
        Wall,
        Ice,
        Goal
    };

    /**
     * Cell type
     */
    Type type;

    /**
     * Flag indicating whether this cell has been visited or not
     */
    bool isVisited;

    /**
     * Current state of the cell
     */
    int32_t state;
};

/**
 * Struct containing data about a room
 */
struct RoomData
{
    /**
     * Grid for the cell data
     */
    Grid<CellData> cells;

    /**
     * Player's starting X-position
     */
    int32_t playerStartX;

    /**
     * Player's starting Y-position
     */
    int32_t playerStartY;

    /**
     * X-position of the goal cell
     */
    int32_t goalX;

    /**
     * Y-position of the goal cell
     */
    int32_t goalY;

    /**
     * List of mappings between switches and doors in the room
     */
    std::map<int32_t, SwitchDoorMapping> switchDoorMappings;

    /**
     * @brief Gets the switch ID from the switch position
     * @param[in] switchX X-position of the switch
     * @param[in] switchY Y-position of the switch
     * @return Switch ID of the switch at the specified position. If the switch at
     * the specified position does not exist in the database, returns -1.
     */
    int32_t GetSwitchIdFromSwitchPosition(const int32_t& switchX, const int32_t& switchY);

    /**
     * @brief Gets the switch ID from the door position
     * @param[in] doorX X-position of the door
     * @param[in] doorY Y-position of the door
     * @return Switch ID of the door at the specified position. If the door at
     * the specified position does not exist in the database, returns -1.
     */
    int32_t GetSwitchIdFromDoorPosition(const int32_t& doorX, const int32_t& doorY);
};

/**
 * Struct containing data about a level
 */
struct LevelData
{
    /**
     * List of room data
     */
    std::vector<RoomData> rooms;

public:
    /**
     * @brief Constructor
     */
    LevelData();

    /**
     * @brief Destructor
     */
    ~LevelData();

    /**
     * @brief Loads the level data from the specified file
     * @param[in] levelFilePath Path to the file containing the level data
     * @return Returns true if the operation was successful. Returns false otherwise
     */
    bool LoadFromFile(const std::string& levelFilePath);
};
