#pragma once

#include "../Common/ISingleAgentPathFinder.h"
#include "RbfsNode.h"
#include <unordered_map>

class Agent;

typedef enum Status
{
    Succeed,
    Failed
}Status;

class RBFS : public ISingleAgentPathFinder
{
public:
    using HashMap = std::unordered_map<Coordinate, RbfsNode, CoordinateHasher>;
    using Solution = std::tuple<double, Status>;

private:
    HashMap Lookup;

    bool IsGenerated(const Coordinate&);
    bool IsNodeExpanded(const RbfsNode&) const;
    bool IsLegalSuccessor(const Coordinate&);
    bool IsSolutionFound(const Solution&) const;
    double ExtractBound(const Solution& sol) const;
    void Expand(const Coordinate, const Coordinate&);
    void Generate(RbfsNode&, const Coordinate&, const Coordinate&);
    Solution Search(const Coordinate, const double, const Coordinate&);
    Path ReconstructPath(const Agent&);

public:
    RBFS(const Heuristic = Euclidean);
    RBFS(Map*, const Heuristic = Euclidean);
    RBFS(Map*, const HeuristicFunction&, const WeightFunction&);
    virtual ~RBFS() = default;
    Path Solve(const Agent&) override;
    Report SolveFullReport(const Agent&) override;
};