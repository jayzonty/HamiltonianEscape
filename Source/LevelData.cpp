#include "LevelData.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#define WALL_SYMBOL '#'
#define START_SYMBOL '*'
#define GOAL_SYMBOL '@'
#define DOOR_SYMBOL '$'
#define FLOOR_SYMBOL '.'

/**
 * @brief Constructor
 */
LevelData::LevelData()
    : cells()
    , rooms()
    , playerStartX(-1)
    , playerStartY(-1)
    , playerPositionX(-1)
    , playerPositionY(-1)
{
}

/**
 * @brief Destructor
 */
LevelData::~LevelData()
{
}

/**
 * @brief Loads the level data from the specified file
 * @param[in] levelFilePath Path to the file containing the level data
 * @return Returns true if the operation was successful. Returns false otherwise
 */
bool LevelData::LoadFromFile(const std::string &levelFilePath)
{
    std::ifstream file(levelFilePath);
    if (file.fail())
    {
        return false;
    }

    int32_t levelWidth, levelHeight;
    file >> levelWidth >> levelHeight;
    if ((levelWidth == 0) || (levelHeight == 0))
    {
        std::cerr << "Level width or height is 0!" << std::endl;
    }

    cells.Resize(levelWidth, levelHeight);

    std::string line;
    std::getline(file, line);
    for (int32_t y = 0; y < levelHeight; ++y)
    {
        std::getline(file, line);
        std::cout << line << std::endl;
        for (int32_t x = 0; x < levelWidth; ++x)
        {
            switch (line[x])
            {
                case FLOOR_SYMBOL:
                    cells.Get(x, y)->type = CellData::Type::Floor;
                    break;
                case WALL_SYMBOL:
                    cells.Get(x, y)->type = CellData::Type::Wall;
                    break;

                case START_SYMBOL:
                    playerStartX = x;
                    playerStartY = y;
                    break;

                case GOAL_SYMBOL:
                    cells.Get(x, y)->type = CellData::Type::Goal;
                    break;

                case DOOR_SYMBOL:
                    cells.Get(x, y)->type = CellData::Type::Door;
                    break;
            }
        }
    }

    rooms.clear();

    int32_t numRooms;
    file >> numRooms;
    for (int32_t i = 0; i < numRooms; ++i)
    {
        rooms.emplace_back();

        RoomData &room = rooms.back();
        file >> room.upperLeftX >> room.upperLeftY >> room.width >> room.height;
        room.isVisited = false;
    }

    return true;
}

/**
 * @brief Gets the index of the room that the specified position falls under
 * @param[in] x X-position
 * @param[in] y Y-position
 * @return Index of the room that the specified position falls under. Returns -1 if
 * the specified position falls on an invalid room.
 */
int32_t LevelData::GetRoomIndexAtPosition(const int32_t& x, const int32_t y)
{
    int32_t numRooms = rooms.size();
    for (int32_t i = 0; i < numRooms; ++i)
    {
        RoomData &room = rooms[i];

        if ((room.upperLeftX <= x) 
            && (x < room.upperLeftX + room.width)
            && (room.upperLeftY <= y)
            && (y < room.upperLeftY + room.height))
        {
            return i;
        }
    }

    return -1;
}
