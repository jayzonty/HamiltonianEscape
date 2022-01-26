#pragma once

#include "Grid.hpp"

#include <cstdint>
#include <string>
#include <vector>

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
        Door,
        Wall,
        Goal
    };

    /**
     * Cell type
     */
    Type type;

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
