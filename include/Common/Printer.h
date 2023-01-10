#pragma once

#include <iostream>
#include <vector>
#include "Coordinate.h"
#include "ISingleAgentPathFinder.h"

using std::cout;
using std::endl;
using std::to_string;

typedef enum OutputColor
{
    Black = 90,
    Red = 91,
    Green = 92,
    Yellow = 93,
    Blue = 94,
    White = 97
}OutputColor;

template<typename First, typename ...Rest>
static inline void DisplayMessage(OutputColor const color, First&& first, Rest&& ...rest)
{
    cout << "\033[1;" << to_string(color) << 'm' << first << "\033[0m";
    DisplayMessage(color, rest...);
}

template<typename First>
static inline void DisplayMessage(OutputColor const color, First&& msg)
{
    cout << "\033[1;" << to_string(color) << 'm' << msg << "\033[0m";
}

static inline void DisplayHeadline(int const bucket_number, int const agent_number)
{
    DisplayMessage(Green, "Solution for agent number: ", agent_number, " belongs to bucket: ", bucket_number, '\n');
}

static inline void DisplayPath(std::vector<Coordinate>& path, int const bucket_number, int const agent_number)
{
    int row_number = 0;
    DisplayHeadline(bucket_number, agent_number);
    for(const auto& coordinate : path)
    {
        DisplayMessage(White, '#', row_number, '\t', coordinate, '\n');
        row_number++;
    }
}

static inline void DisplayPath(const std::vector<Coordinate>& path)
{
    int row_number = 0;
    DisplayMessage(Green, "Solution contains ", path.size(), " steps:\n");
    for(const auto& coordinate : path)
    {
        DisplayMessage(White, '#', row_number, '\t', coordinate, '\n');
        row_number++;
    }
}

static inline void DisplayReport(Report& report)
{
    constexpr int PATH = 0, AGENT = 1, NEXPANDED = 2, NGENERAED = 3, NPOPED = 4, NHEAP = 5;
    Path& path = get<PATH>(report);
    const Agent& agent = get<AGENT>(report);

    bool is_solution_found = !path.empty();
    if(is_solution_found)
    {
        DisplayMessage(Green, "Succeed to find path from ", agent.GetStartCoordinate(), " to ", agent.GetGoalCoordinate(), '\n');
    }
    else
    {
        DisplayMessage(Red, "Failed to find path from ", agent.GetStartCoordinate(), " to ", agent.GetGoalCoordinate(), '\n');
    }

    DisplayMessage(White, "Number of expanded nodes: ", get<NEXPANDED>(report), '\n');
    DisplayMessage(White, "Number of generated nodes: ", get<NGENERAED>(report), '\n');
    DisplayMessage(White, "Number of pop operations: ", get<NPOPED>(report), '\n');
    DisplayMessage(White, "Max heap size: ", get<NHEAP>(report), '\n');
    DisplayPath(path);
}

static inline void DisplayInvalidCoordinateMessage(Coordinate const& coordinate, int const nrows, int const ncolumns)
{
    DisplayMessage(Red, "Invalid coordinate supplied: ", coordinate, " Grid shape is: [", nrows, "][", ncolumns, "]\n");
}

static inline void DisplayMessage(std::string&& msg)
{
    std::cout << msg << std::endl;
}
