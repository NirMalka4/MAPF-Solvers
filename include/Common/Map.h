#pragma once

#include <vector> // grid_t
#include "Coordinate.h"
class Agent;

class Map
{
private:
    using grid_t = std::vector<std::vector<unsigned char>>;
    grid_t Grid;
    int NumberOfRows, NumberOfColumns;

public:
    Map();
    Map(char* const);
    virtual ~Map() = default;

    bool IsValidCoordinate(Coordinate const&);
    bool IsPassableCoordinate(Coordinate const&);
    bool Load(char const*);
    void SetAgent(Agent const&);
    void RemoveAgent(Agent const&);
    std::string GetGridWithSolution(std::vector<Coordinate>&, const Agent&);
    std::string GetGrid(void) const;
};