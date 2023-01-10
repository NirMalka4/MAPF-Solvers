#include "../../include/AStar/AStar.h"
#include "../../include/Common/Map.h"
#include "../../include/Common/Agent.h"
#include "../../include/Common/Printer.h"
#include "../../include/Common/Directions.h"
#include <cmath>

const double POSITIVE_INFINITY = std::numeric_limits<double>::max();

AStar::AStar(const Heuristic heuristic): ISingleAgentPathFinder(heuristic), Lookup() {}

AStar::AStar(Map* map, const Heuristic heuristic): ISingleAgentPathFinder(map, heuristic), Lookup() {}

AStar::AStar(Map* map, const HeuristicFunction &heuristic, const WeightFunction &weight):
        ISingleAgentPathFinder(map, heuristic, weight), Lookup() {}

bool AStar::IsNodeExpanded(const Coordinate& coordinate)
{
    if(Lookup.find(coordinate) != Lookup.end())
    {
        return Lookup[coordinate].IsExpanded;
    }
    return false;
}

bool AStar::IsLegalSuccessor(const Coordinate& successor_coordinate)
{
    return CurrentMap->IsValidCoordinate(successor_coordinate) &&
           CurrentMap->IsPassableCoordinate(successor_coordinate) &&
           !IsNodeExpanded(successor_coordinate);
}

void AStar::Generate(AStarNode* root_node, const Coordinate& successor_coordinate, const Coordinate& goal, heap_t& open_set)
{
    // calculate static value(f) , sum of weights(g), stored value(F) for successor
    const Coordinate& root_coordinate = root_node->MyCoordinate;
    AStarNode& successor_node = Lookup[successor_coordinate];
    double successor_sum_of_weights = root_node->SumOfWeights + W(root_coordinate, successor_coordinate);
    double successor_heuristic_estimation = H(successor_coordinate, goal);
    double successor_static_value = successor_sum_of_weights + successor_heuristic_estimation;

    if(successor_node.IsGenerated)
    {
        // found shorter path to successor since heuristic estimation is fixed.
        if(successor_node.SumOfWeights > successor_sum_of_weights)
        {
            successor_node.SumOfWeights = successor_sum_of_weights;
            successor_node.StaticValue = successor_static_value;
            successor_node.Parent = root_coordinate;
            open_set.decrease(successor_node.HeapPointer, &successor_node);
        }
    }
    else
    {
        NumberOfGeneratedNodes++;
        successor_node = {successor_coordinate, root_coordinate, successor_static_value, successor_sum_of_weights};
        successor_node.IsGenerated = true;
        successor_node.HeapPointer = open_set.emplace(&successor_node);
    }

}

void AStar::Expand(AStarNode* root_node, const Coordinate& goal, heap_t& open_set)
{
    NumberOfExpandedNodes++;
    const Coordinate& root_coordinate = root_node->MyCoordinate;
    for(const auto& direction : eight_principle_directions)
    {
        Coordinate successor_coordinate = {root_coordinate.GetRow() + direction.GetRow(),
                                           root_coordinate.GetColumn() + direction.GetColumn()};

        if(IsLegalSuccessor(successor_coordinate))
        {
            Generate(root_node, successor_coordinate, goal, open_set);
        }
    }
    root_node->IsExpanded = true;
}

bool AStar::Search(const Coordinate root_coordinate, const Coordinate& goal)
{
    // create AStarNode for root and insert in to Lookup table
    heap_t open_set;
    const double root_heuristic_estimation = H(root_coordinate, goal);
    Lookup[root_coordinate] = {root_coordinate, root_heuristic_estimation, 0};
    AStarNode& root_node = Lookup[root_coordinate];
    //
    NumberOfExpandedNodes = NumberOfGeneratedNodes = NumberOfPopOperations = MaxHeapSize = 0;

    //insert pointer to root AStarNode (at Lookup) to open-set
    root_node.HeapPointer = open_set.emplace(&root_node);
    while(!open_set.empty())
    {
        AStarNode* curr = open_set.top();
        MaxHeapSize = std::max(open_set.size(), MaxHeapSize);

        open_set.pop();
        NumberOfPopOperations++;

        if(IsGoal(curr->MyCoordinate, goal))
        {
            return true;
        }

        Expand(curr, goal, open_set);
    }
    return false;
}

Path AStar::ReconstructPath(const Agent& agent)
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

Path AStar::Solve(const Agent& agent)
{
    if(CurrentMap == nullptr)
    {
        DisplayMessage(Red, __PRETTY_FUNCTION__ , ": CurrentMap is nulltptr!\n");
        return {};
    }
    Lookup.clear();
    const Coordinate src = agent.GetStartCoordinate(), dst = agent.GetGoalCoordinate();
    bool is_solution_found = Search(src, dst);
    if(is_solution_found)
    {
        return ReconstructPath(agent);
    }
    return {};
}

Report AStar::SolveFullReport(const Agent& agent)
{
    if(CurrentMap == nullptr)
    {
        DisplayMessage(Red, __PRETTY_FUNCTION__ , ": CurrentMap is nulltptr!\n");
        return {};
    }
    Lookup.clear();
    NumberOfExpandedNodes = NumberOfGeneratedNodes = NumberOfPopOperations = MaxHeapSize = 0;
    const Coordinate src = agent.GetStartCoordinate(), dst = agent.GetGoalCoordinate();
    bool is_solution_found = Search(src, dst);
    Path path;
    if(is_solution_found)
    {
        path = ReconstructPath(agent);
    }
    return {path, agent, NumberOfExpandedNodes, NumberOfGeneratedNodes, NumberOfPopOperations, MaxHeapSize};
}