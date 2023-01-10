#pragma once

#include "../Common/ISingleAgentPathFinder.h"
#include "PEAStarNode.h"
#include <unordered_map>

class Agent;
class Map;

struct PEAStarNodeComparator
{
    bool operator() (const PEAStarNode*, const PEAStarNode*) const;
};

class PEAStar : public ISingleAgentPathFinder
{
public:
    using HashMap = std::unordered_map<Coordinate, PEAStarNode, CoordinateHasher>;
    using binomial_heap_t = boost::heap::binomial_heap<PEAStarNode*, boost::heap::compare<PEAStarNodeComparator>>;

private:
    // open_set contains pointers to Nodes in the Lookup table
    HashMap Lookup;

    bool IsNodeExpanded(const Coordinate&);
    bool IsNodeWithinOpenSet(const Coordinate&);
    bool IsLegalSuccessor(const Coordinate&);
    void Collapse(PEAStarNode*, const double, binomial_heap_t&);
    void Expand(PEAStarNode*, const Coordinate&, binomial_heap_t&);
    double Generate(PEAStarNode*, const Coordinate&, const Coordinate&, binomial_heap_t&);
    bool Search(const Coordinate, const Coordinate&);
    Path ReconstructPath(const Agent&);

public:
    PEAStar(const Heuristic = Euclidean);
    PEAStar(Map*, const Heuristic = Euclidean);
    PEAStar(Map*, const HeuristicFunction&, const WeightFunction&);
    virtual ~PEAStar() = default;
    Path Solve(const Agent&) override;
    Report SolveFullReport(const Agent&) override;
};