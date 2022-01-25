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
     * X-coordinate of the room's upper-left corner
     */
    int32_t upperLeftX;

    /**
     * Y-coordinate of the room's upper-left corner
     */
    int32_t upperLeftY;

    /**
     * Room width
     */
    int32_t width;

    /**
     * Room height
     */
    int32_t height;

    /**
     * Flag indicating whether this room has been visited or not
     */
    bool isVisited;
};

/**
 * Struct containing data about a level
 */
struct LevelData
{
    /**
     * Grid for the cell data
     */
    Grid<CellData> cells;

    /**
     * List of room data
     */
    std::vector<RoomData> rooms;

    /**
     * Player's starting X-position
     */
    int32_t playerStartX;

    /**
     * Player's starting Y-position
     */
    int32_t playerStartY;

    /**
     * Player's current X-position
     */
    int32_t playerPositionX;

    /**
     * Player's current Y-position
     */
    int32_t playerPositionY;

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

    /**
     * @brief Gets the index of the room that the specified position falls under
     * @param[in] x X-position
     * @param[in] y Y-position
     * @return Index of the room that the specified position falls under. Returns -1 if
     * the specified position falls on an invalid room.
     */
    int32_t GetRoomIndexAtPosition(const int32_t& x, const int32_t y);
};
