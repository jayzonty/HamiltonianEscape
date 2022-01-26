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
    : rooms()
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

    rooms.clear();

    int32_t numRooms;
    file >> numRooms;
    for (int32_t i = 0; i < numRooms; ++i)
    {
        int32_t roomWidth, roomHeight;
        file >> roomWidth >> roomHeight;
        if ((roomWidth == 0) || (roomHeight == 0))
        {
            std::cerr << "Room width or height is 0!" << std::endl;
        }

        rooms.emplace_back();

        RoomData &room = rooms.back();
        room.cells.Resize(roomWidth, roomHeight);

        std::string line;
        std::getline(file, line);
        for (int32_t y = 0; y < roomHeight; ++y)
        {
            std::getline(file, line);
            std::cout << line << std::endl;
            for (int32_t x = 0; x < roomWidth; ++x)
            {
                switch (line[x])
                {
                    case FLOOR_SYMBOL:
                        room.cells.Get(x, y)->type = CellData::Type::Floor;
                        break;
                    case WALL_SYMBOL:
                        room.cells.Get(x, y)->type = CellData::Type::Wall;
                        break;

                    case GOAL_SYMBOL:
                        room.cells.Get(x, y)->type = CellData::Type::Goal;
                        room.goalX = x;
                        room.goalY = y;
                        break;

                    case DOOR_SYMBOL:
                        room.cells.Get(x, y)->type = CellData::Type::Door;
                        break;
                    
                    default:
                        room.cells.Get(x, y)->type = CellData::Type::Empty;
                        break;
                }
            }
        }   

        file >> room.playerStartX >> room.playerStartY;
    }

    return true;
}
