#pragma once

#include "../Common/ISingleAgentPathFinder.h"
#include "AStarNode.h"
#include <unordered_map>

class Agent;
class Map;

class AStar : public ISingleAgentPathFinder
{
public:
    using HashMap = std::unordered_map<Coordinate, AStarNode, CoordinateHasher>;

private:
    // open_set contains pointers to AStarNode in the Lookup table
    HashMap Lookup;

    bool IsNodeExpanded(const Coordinate&);
    bool IsLegalSuccessor(const Coordinate&);
    void Expand(AStarNode*, const Coordinate&, heap_t&);
    void Generate(AStarNode*, const Coordinate&, const Coordinate&, heap_t&);
    bool Search(const Coordinate, const Coordinate&);
    Path ReconstructPath(const Agent&);

public:
    AStar(const Heuristic = Euclidean);
    AStar(Map*, const Heuristic = Euclidean);
    AStar(Map*, const HeuristicFunction&, const WeightFunction&);
    virtual ~AStar() = default;
    Path Solve(const Agent&) override;
    Report SolveFullReport(const Agent&) override;
};