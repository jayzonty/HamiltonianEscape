#include "LevelData.hpp"

#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#define WALL_SYMBOL '#'
#define GOAL_SYMBOL '@'
#define FLOOR_SYMBOL '.'
#define ICE_SYMBOL '*'

/**
 * @brief Gets the switch ID from the switch position
 * @param[in] switchX X-position of the switch
 * @param[in] switchY Y-position of the switch
 * @return Switch ID of the switch at the specified position. If the switch at
 * the specified position does not exist in the database, returns -1.
 */
int32_t RoomData::GetSwitchIdFromSwitchPosition(const int32_t &switchX, const int32_t &switchY)
{
    for (auto &it : switchDoorMappings)
    {
        if ((it.second.switchX == switchX) && (it.second.switchY == switchY))
        {
            return it.first;
        }
    }

    return -1;
}

/**
 * @brief Gets the switch ID from the door position
 * @param[in] doorX X-position of the door
 * @param[in] doorY Y-position of the door
 * @return Switch ID of the door at the specified position. If the door at
 * the specified position does not exist in the database, returns -1.
 */
int32_t RoomData::GetSwitchIdFromDoorPosition(const int32_t& doorX, const int32_t& doorY)
{
    for (auto &it : switchDoorMappings)
    {
        if ((it.second.doorX == doorX) && (it.second.doorY == doorY))
        {
            return it.first;
        }
    }

    return -1;
}

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
            for (int32_t x = 0; x < roomWidth; ++x)
            {
                if (std::isalpha(line[x]))
                {
                    int32_t switchId = static_cast<int32_t>(std::tolower(line[x]));

                    if (std::islower(line[x]))
                    {
                        room.switchDoorMappings[switchId].switchX = x;
                        room.switchDoorMappings[switchId].switchY = y;
                        room.cells.Get(x, y)->type = CellData::Type::Switch;
                    }
                    else
                    {
                        room.switchDoorMappings[switchId].doorX = x;
                        room.switchDoorMappings[switchId].doorY = y;
                        room.cells.Get(x, y)->type = CellData::Type::Door;
                    }
                }
                else
                {
                    switch (line[x])
                    {
                        case FLOOR_SYMBOL:
                            room.cells.Get(x, y)->type = CellData::Type::Floor;
                            break;
                        case WALL_SYMBOL:
                            room.cells.Get(x, y)->type = CellData::Type::Wall;
                            break;

                        case ICE_SYMBOL:
                            room.cells.Get(x, y)->type = CellData::Type::Ice;
                            break;

                        case GOAL_SYMBOL:
                            room.cells.Get(x, y)->type = CellData::Type::Goal;
                            room.goalX = x;
                            room.goalY = y;
                            break;

                        default:
                            room.cells.Get(x, y)->type = CellData::Type::Empty;
                            break;
                    }
                }
                
            }
        }   

        file >> room.playerStartX >> room.playerStartY;
    }

    return true;
}
