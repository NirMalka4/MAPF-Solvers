#include "../../include/RBFS/RBFS.h"
#include "../../include/Common/Map.h"
#include "../../include/Common/Agent.h"
#include "../../include/Common/Printer.h"
#include "../../include/Common/Directions.h"
#include <cmath>

const double POSITIVE_INFINITY = std::numeric_limits<double>::max();

RBFS::RBFS(const Heuristic heuristic): ISingleAgentPathFinder(heuristic), Lookup() {}

RBFS::RBFS(Map* map, const Heuristic heuristic): ISingleAgentPathFinder(map, heuristic), Lookup() {}

RBFS::RBFS(Map* map, const HeuristicFunction &heuristic, const WeightFunction &weight):
ISingleAgentPathFinder(map, heuristic, weight), Lookup() {}

bool RBFS::IsGenerated(const Coordinate& coordinate)
{
    return Lookup.find(coordinate) != Lookup.end();
}

bool RBFS::IsLegalSuccessor(const Coordinate& successor_coordinate)
{
    return  CurrentMap->IsValidCoordinate(successor_coordinate) &&
            CurrentMap->IsPassableCoordinate(successor_coordinate) &&
            !IsGenerated(successor_coordinate);
}

Path RBFS::Solve(const Agent& agent)
{
    if(CurrentMap == nullptr)
    {
        DisplayMessage(Red, __PRETTY_FUNCTION__ , ": CurrentMap is nullptr!\n");
        return {};
    }
    Lookup.clear();
    const Coordinate src = agent.GetStartCoordinate(), dst = agent.GetGoalCoordinate();
    const double root_stored_value = H(src, dst);
    Lookup[src] = {src, root_stored_value, root_stored_value, 0};
    auto sol = Search(src ,root_stored_value, dst);
    if(IsSolutionFound(sol))
    {
        Path solution = ReconstructPath(agent);
        return solution;
    }
    return {};
}

bool RBFS::IsNodeExpanded(const RbfsNode& RbfsNode) const
{
    return RbfsNode.StoredValue > RbfsNode.StaticValue;
}

bool RBFS::IsSolutionFound(const Solution& sol) const
{
    constexpr int STATUS_INDEX = 1;
    return get<STATUS_INDEX>(sol) == Succeed;
}

double RBFS::ExtractBound(const Solution& sol) const
{
    constexpr int BOUND_INDEX = 0;
    return get<BOUND_INDEX>(sol);
}

void RBFS::Generate(RbfsNode& root_node, const Coordinate& successor_coordinate, const Coordinate& goal)
{
    NumberOfGeneratedNodes++;
    // calculate static value(f) , sum of weights(g), stored value(F) for successor
    const Coordinate& root_coordinate = root_node.MyCoordinate;
    RbfsNode& successor_node = Lookup[successor_coordinate];
    double successor_sum_of_weights = root_node.SumOfWeights + W(root_coordinate, successor_coordinate);
    double successor_heuristic_estimation = H(successor_coordinate, goal);
    double successor_static_value = successor_sum_of_weights + successor_heuristic_estimation;
    successor_node = {successor_coordinate, root_coordinate, successor_static_value, successor_static_value, successor_sum_of_weights};

    // add successor RbfsNode address to its parent successors vector
    root_node.Successors.push_back(&successor_node);
}

void RBFS::Expand(const Coordinate root_coordinate, const Coordinate& goal)
{
    RbfsNode& root_node = Lookup[root_coordinate];
    if(!IsNodeExpanded(root_node))
    {
        NumberOfExpandedNodes++;
        for(const auto& direction : eight_principle_directions)
        {
            Coordinate successor_coordinate = {root_coordinate.GetRow() + direction.GetRow(),
                                               root_coordinate.GetColumn() + direction.GetColumn()};

            if(IsLegalSuccessor(successor_coordinate))
            {
                Generate(root_node, successor_coordinate, goal);
            }
        }
    }
    else
    {   // apply restore action
        for(RbfsNode* successor : root_node.Successors)
        {
            successor->StoredValue = std::max(root_node.StoredValue, successor->StaticValue);
        }
    }

    std::sort(root_node.Successors.begin(), root_node.Successors.end(), RbfsNode::RbfsNodeComparator);
}

RBFS::Solution RBFS::Search(const Coordinate root_coordinate, const double bound, const Coordinate& goal)
{
    if(IsGoal(root_coordinate, goal))
    {
        return {bound, Succeed};
    }

    NumberOfPopOperations++;

    Expand(root_coordinate, goal);
    RbfsNode& root_node = Lookup[root_coordinate];
    std::vector<RbfsNode*>& successors = root_node.Successors;
    auto number_of_successors = successors.size();

    if(number_of_successors == 0)
    {
        return {POSITIVE_INFINITY, Failed};
    }

    RbfsNode* best_successor = successors.at(0);
    double best_successor_stored_value = best_successor->StoredValue;

    while(best_successor_stored_value <= bound && best_successor_stored_value < POSITIVE_INFINITY)
    {
        const double alternative = (number_of_successors == 1) ? POSITIVE_INFINITY : successors.at(1)->StoredValue;
        auto solution = Search(best_successor->MyCoordinate, std::min(bound, alternative), goal);
        // solution is found and cross through best_successor RbfsNode.
        if(IsSolutionFound(solution))
        {
            return solution;
        }
        // apply collapse action
        // search for a solution in a different branch
        best_successor->StoredValue = ExtractBound(solution);
        std::sort(successors.begin(), successors.end(), RbfsNode::RbfsNodeComparator);
        best_successor = successors.at(0);
        best_successor_stored_value = best_successor->StoredValue;
    }
    return {best_successor_stored_value, Failed};
}

Path RBFS::ReconstructPath(const Agent& agent)
{
    Coordinate current = Lookup[agent.GetGoalCoordinate()].MyCoordinate;
    const Coordinate& source = Lookup[agent.GetStartCoordinate()].MyCoordinate;
    Path solution;

    while(current != source)
    {
        solution.emplace_back(current);
        current = Lookup[current].Parent;
    }

    solution.push_back(source);
    std::reverse(solution.begin(), solution.end());

    return solution;
}

Report RBFS::SolveFullReport(const Agent& agent)
{
    if(CurrentMap == nullptr)
    {
        DisplayMessage(Red, __PRETTY_FUNCTION__ , ": CurrentMap is nullptr!\n");
        return {};
    }
    Lookup.clear();
    NumberOfExpandedNodes = NumberOfGeneratedNodes = NumberOfPopOperations = MaxHeapSize = 0;
    const Coordinate src = agent.GetStartCoordinate(), dst = agent.GetGoalCoordinate();
    const double root_stored_value = H(src, dst);
    Lookup[src] = {src, root_stored_value, root_stored_value, 0};
    auto sol = Search(src ,POSITIVE_INFINITY, dst);
    Path solution;
    if(IsSolutionFound(sol))
    {
        solution = ReconstructPath(agent);
    }
    return {solution, agent, NumberOfExpandedNodes, NumberOfGeneratedNodes, NumberOfPopOperations, MaxHeapSize};
}