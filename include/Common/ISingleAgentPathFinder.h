#pragma once

#include <vector> // Path
#include <functional>
#include "Coordinate.h"

class Map;
class Agent;

using Path = std::vector<Coordinate>;
using HeuristicFunction = std::function<double(const Coordinate&, const Coordinate&)>;
using WeightFunction  = std::function<double(const Coordinate&, const Coordinate&)>;
using Report = std::tuple<Path, Agent, unsigned int, unsigned int, unsigned int, unsigned long>;

typedef enum Heuristic
{
    Euclidean,
    Manhattan,
    NHeuristic
}Heuristic;


class ISingleAgentPathFinder
{
protected:
    Map* CurrentMap;
    HeuristicFunction H;
    WeightFunction W;
    unsigned int NumberOfExpandedNodes, NumberOfGeneratedNodes, NumberOfPopOperations;
    unsigned long MaxHeapSize;
    bool IsGoal(const Coordinate&, const Coordinate&);

public:
    void SetMap(Map*);
    void SetHeuristic(const Heuristic = Euclidean);
    ISingleAgentPathFinder(const Heuristic = Euclidean);
    ISingleAgentPathFinder(Map*, const Heuristic = Euclidean);
    ISingleAgentPathFinder(Map*, const HeuristicFunction&, const WeightFunction&);
    virtual ~ISingleAgentPathFinder() = default;
    virtual Path Solve(const Agent&) = 0;
    virtual Report SolveFullReport(const Agent&) = 0;
};