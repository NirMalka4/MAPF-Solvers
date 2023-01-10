#include "../../include/Common/Map.h"
#include "../../include/Common/Agent.h" // GetStartCoordinate(), GetGoalCoordinate()
#include "../../include/Common/Printer.h" // DisplayInvalidCoordinateMessage()
#include <algorithm> // any_of()
#include <fstream>// ifstream, ofstream
#include <sstream> // GetGrid()

Map::Map() : NumberOfRows(0), NumberOfColumns(0), Grid() {}
Map::Map(char* const path) : NumberOfRows(0), NumberOfColumns(0), Grid() { Load(path);}

bool Map::IsValidCoordinate(Coordinate const& coordinate)
{
    return coordinate.GetRow() < NumberOfRows && coordinate.GetRow() >= 0 && coordinate.GetColumn() >= 0 && coordinate.GetColumn() < NumberOfColumns;
}

bool Map::IsPassableCoordinate(Coordinate const& coordinate)
{
    constexpr size_t NUMBER_OF_PASSABLE_TERRAIN = 4;
    std::array<unsigned char, NUMBER_OF_PASSABLE_TERRAIN> passable_terrains = {'.', 'G', 'S', 'W'};
    
    auto terrain = Grid[coordinate.GetRow()][coordinate.GetColumn()];
    return std::any_of(passable_terrains.begin(), passable_terrains.end(), [&](const auto& t){return terrain == t;});
}

bool Map::Load(const char *path)
{
    std::ifstream file(path, std::ios::in);

    if(!file)
    {
        return false;
    }

    std::string type, space, octile, height, width, map, line;
    /*
        All maps begin with the lines:
        type octile
        height y
        width x
        map
    */
    file >> type >> octile >> height >> NumberOfRows >> width >> NumberOfColumns >> map;

    int row = 0, i = 0;
    constexpr unsigned char NUMBER_OF_CARRIAGE_LITERALS = 32;
    bool advance_next_row = false;
    Grid.resize(NumberOfRows);
    while(getline(file, line))
    {
        Grid[row].resize(NumberOfColumns);
        for(const auto& tile : line)
        {
            if(tile > NUMBER_OF_CARRIAGE_LITERALS)
            {
                Grid[row][i++] = tile;
                advance_next_row = true;
            }
        }
        if(advance_next_row)
        {
            row++;
            i = 0;
        }
        advance_next_row = false;
    }
    file.close();
    return true;
}

void Map::SetAgent(Agent const& agent)
{
    constexpr unsigned char AGENT_MARKER = 'A';
    constexpr unsigned char GOAL_MARKER = 'G';
    
    Coordinate const& start_coordinate = agent.GetStartCoordinate(), goal_coordinate = agent.GetGoalCoordinate();
    if(!IsValidCoordinate(start_coordinate))
    {
        DisplayInvalidCoordinateMessage(start_coordinate, NumberOfRows, NumberOfColumns);
        return;
    }
    if(!IsValidCoordinate(goal_coordinate))
    {
        DisplayInvalidCoordinateMessage(start_coordinate, NumberOfRows, NumberOfColumns);
        return;
    }
    Grid[start_coordinate.GetRow()][start_coordinate.GetColumn()] = AGENT_MARKER;
    Grid[goal_coordinate.GetRow()][goal_coordinate.GetColumn()] = GOAL_MARKER;
}

void Map::RemoveAgent(Agent const& agent)
{
    static constexpr unsigned char EMPTY_TERRAIN_MARKER = '.';
    Coordinate const& start_coordinate = agent.GetStartCoordinate(), goal_coordinate = agent.GetGoalCoordinate();
    if(!IsValidCoordinate(start_coordinate))
    {
        DisplayInvalidCoordinateMessage(start_coordinate, NumberOfRows, NumberOfColumns);
        return;
    }
    if(!IsValidCoordinate(goal_coordinate))
    {
        DisplayInvalidCoordinateMessage(start_coordinate, NumberOfRows, NumberOfColumns);
        return;
    }
    Grid[start_coordinate.GetRow()][start_coordinate.GetColumn()] = EMPTY_TERRAIN_MARKER;
    Grid[goal_coordinate.GetRow()][goal_coordinate.GetColumn()] = EMPTY_TERRAIN_MARKER;
}

std::string Map::GetGrid(void) const
{
    std::stringstream ss;
    //print columns number
    ss << "    ";
    for(int i = 0; i < NumberOfColumns; i++)
    {
        ss << i << ((i < 10) ? "  " : " ");
    }
    ss << '\n';

    for(int i = 0; i < NumberOfRows; i++)
    {
        // print rows number
        ss << i << ((i < 10) ? "   " : (i < 100) ? "  " : " ");
        for(int j = 0; j < NumberOfColumns; j++)
        {
            ss << Grid[i][j]<< ((j < 100) ? "  " : "   ");
        }
        ss << '\n';
    }
    return ss.str();
}

std::string Map::GetGridWithSolution(std::vector<Coordinate>& solution, const Agent& agent)
{
    SetAgent(agent);
    std::stringstream ss;
    //print columns number
    ss << "    ";
    for(int i = 0; i < NumberOfColumns; i++)
    {
        ss << i << ((i < 10) ? "  " : " ");
    }
    ss << '\n';

    for(int i = 0; i < NumberOfRows; i++)
    {
        // print rows number
        ss << i << ((i < 10) ? "   " : (i < 100) ? "  " : " ");
        for(int j = 0; j < NumberOfColumns; j++)
        {
            if(std::find(solution.begin(), solution.end(), Coordinate(i, j)) != solution.end())
            {
                // display solution coordinates with red color
                ss << "\033[1;91m" << Grid[i][j] << "\033[0m" <<((j < 100) ? "  " : "   ");
            }
            else
            {
                ss << Grid[i][j]<< ((j < 100) ? "  " : "   ");
            }
        }
        ss << '\n';
    }
    RemoveAgent(agent);
    return ss.str();
}