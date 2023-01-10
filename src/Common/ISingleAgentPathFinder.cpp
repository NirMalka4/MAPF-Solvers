#include "../../include/Common/ISingleAgentPathFinder.h"
#include <cmath>
#include "../../include/Common/Agent.h"

using std::sqrt;
using std::pow;

static inline double EuclideanDistance(Coordinate const& src, Coordinate const& dst)
{
    return sqrt(pow(src.GetRow() - dst.GetRow(), 2.0) + pow(src.GetColumn() - dst.GetColumn(), 2.0));
}

static inline double ManhattanDistance(Coordinate const& src, Coordinate const& dst)
{
    return abs(src.GetRow() - dst.GetRow()) + abs(src.GetColumn() - dst.GetColumn());
}

static inline double DefaultWeightFunction(const Coordinate& src, const Coordinate& dst)
{
    return src != dst ? 1 : 0;
}
std::array<HeuristicFunction, NHeuristic> HeuristicsFunctions = {EuclideanDistance, ManhattanDistance};

ISingleAgentPathFinder::ISingleAgentPathFinder(const Heuristic heuristic):
    CurrentMap(nullptr), H(HeuristicsFunctions[heuristic]), W(DefaultWeightFunction),
    NumberOfExpandedNodes(0), NumberOfGeneratedNodes(0), NumberOfPopOperations(0), MaxHeapSize(0){}

ISingleAgentPathFinder::ISingleAgentPathFinder(Map* map, const Heuristic heuristic):
    CurrentMap(map), H(HeuristicsFunctions[heuristic]), W(DefaultWeightFunction),
    NumberOfExpandedNodes(0), NumberOfGeneratedNodes(0), NumberOfPopOperations(0), MaxHeapSize(0){}

ISingleAgentPathFinder::ISingleAgentPathFinder(Map* map,
                                               const HeuristicFunction& heuristic,
                                               const WeightFunction& weight):
    CurrentMap(map), H(heuristic), W(weight), NumberOfExpandedNodes(0),
    NumberOfGeneratedNodes(0), NumberOfPopOperations(0), MaxHeapSize(0){}

bool ISingleAgentPathFinder::IsGoal(const Coordinate& curr, const Coordinate& dst)
{
    return curr == dst;
}

void ISingleAgentPathFinder::SetMap(Map* new_map)
{
    CurrentMap = new_map;
}

void ISingleAgentPathFinder::SetHeuristic(const Heuristic heuristic)
{
    H = HeuristicsFunctions[heuristic];
}