#include "../../include/PEAStar/PEAStar.h"
#include "../../include/Common/Map.h"
#include "../../include/Common/Agent.h"
#include "../../include/Common/Printer.h"
#include "../../include/Common/Directions.h"
#include <cmath>

const double POSITIVE_INFINITY = std::numeric_limits<double>::max();

PEAStar::PEAStar(const Heuristic heuristic): ISingleAgentPathFinder(heuristic), Lookup() {}

PEAStar::PEAStar(Map* map, const Heuristic heuristic): ISingleAgentPathFinder(map, heuristic), Lookup() {}

PEAStar::PEAStar(Map* map, const HeuristicFunction &heuristic, const WeightFunction &weight):
        ISingleAgentPathFinder(map, heuristic, weight), Lookup() {}

bool PEAStar::IsNodeExpanded(const Coordinate& coordinate)
{
    if(Lookup.find(coordinate) != Lookup.end())
    {
        return Lookup[coordinate].IsExpanded;
    }
    return false;
}

bool PEAStar::IsNodeWithinOpenSet(const Coordinate& coordinate)
{
    if(Lookup.find(coordinate) != Lookup.end())
    {
        return Lookup[coordinate].IsOpen;
    }
    return false;
}

bool PEAStar::IsLegalSuccessor(const Coordinate& successor_coordinate)
{
    return CurrentMap->IsValidCoordinate(successor_coordinate) &&
           CurrentMap->IsPassableCoordinate(successor_coordinate) &&
           !IsNodeExpanded(successor_coordinate) && // successor belongs to close-set
           !IsNodeWithinOpenSet(successor_coordinate); // to avoid infinite loops when node is re-expanded
}

double PEAStar::Generate(PEAStarNode* root_node, const Coordinate& successor_coordinate, const Coordinate& goal, binomial_heap_t& open_set)
{
    NumberOfGeneratedNodes++;
    // calculate static value(f) , sum of weights(g), stored value(F) for successor
    const Coordinate& root_coordinate = root_node->MyCoordinate;
    PEAStarNode& successor_node = Lookup[successor_coordinate];
    double successor_sum_of_weights = root_node->SumOfWeights + W(root_coordinate, successor_coordinate);
    double successor_heuristic_estimation = H(successor_coordinate, goal);
    double successor_static_value = successor_sum_of_weights + successor_heuristic_estimation;
    successor_node = {successor_coordinate, root_coordinate, successor_static_value, successor_static_value, successor_sum_of_weights};

    if(successor_static_value <= root_node->StoredValue)
    {
        open_set.push(&successor_node);
        successor_node.IsOpen = true;
        return POSITIVE_INFINITY;
    }
    else
    {
        return successor_static_value;
    }

}

void PEAStar::Collapse(PEAStarNode *root_node, const double least_successor_static_value, binomial_heap_t& open_set)
{
    if(least_successor_static_value == POSITIVE_INFINITY)
    {
        root_node->IsExpanded = true;
    }
    else
    {
        root_node->StoredValue = least_successor_static_value;
        root_node->IsOpen = true;
        open_set.push(root_node);
    }
}

void PEAStar::Expand(PEAStarNode* root_node, const Coordinate& goal, binomial_heap_t& open_set)
{
    NumberOfExpandedNodes++;
    root_node->IsOpen = false;
    const Coordinate& root_coordinate = root_node->MyCoordinate;
    double least_successor_static_value = POSITIVE_INFINITY;
    for(const auto& direction : eight_principle_directions)
    {
        Coordinate successor_coordinate = {root_coordinate.GetRow() + direction.GetRow(),
                                           root_coordinate.GetColumn() + direction.GetColumn()};

        if(IsLegalSuccessor(successor_coordinate))
        {
            least_successor_static_value = std::min(least_successor_static_value,
                                                    Generate(root_node, successor_coordinate, goal, open_set));
        }
    }
    Collapse(root_node, least_successor_static_value, open_set);
}

bool PEAStar::Search(const Coordinate root_coordinate, const Coordinate& goal)
{
    // create PEAStarNode for root and insert in to both Lookup table and open set
    binomial_heap_t open_set;
    const double root_heuristic_estimation = H(root_coordinate, goal);
    Lookup[root_coordinate] = {root_coordinate, root_heuristic_estimation, root_heuristic_estimation, 0};
    PEAStarNode& root_node = Lookup[root_coordinate];
    open_set.push(&root_node);
    root_node.IsOpen = true;

    while(!open_set.empty())
    {
        PEAStarNode* curr = open_set.top();
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

Path PEAStar::ReconstructPath(const Agent& agent)
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

Path PEAStar::Solve(const Agent& agent)
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

Report PEAStar::SolveFullReport(const Agent& agent)
{
    if(CurrentMap == nullptr)
    {
        DisplayMessage(Red, __PRETTY_FUNCTION__ , ": CurrentMap is nulltptr!\n");
        return {};
    }
    Lookup.clear();
    const Coordinate src = agent.GetStartCoordinate(), dst = agent.GetGoalCoordinate();
    NumberOfExpandedNodes = NumberOfGeneratedNodes = NumberOfPopOperations = MaxHeapSize = 0;
    bool is_solution_found = Search(src, dst);
    Path path;
    if(is_solution_found)
    {
        path = ReconstructPath(agent);
    }
    return {path, agent, NumberOfExpandedNodes, NumberOfGeneratedNodes, NumberOfPopOperations, MaxHeapSize};
}

bool PEAStarNodeComparator::operator()(const PEAStarNode *n1, const PEAStarNode *n2) const
{
    return (n1->StaticValue == n2->StaticValue) ? (n1->SumOfWeights > n2->SumOfWeights) :
           (n1->StaticValue > n2->StaticValue);
}