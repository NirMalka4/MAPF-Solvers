#include "../../include/AStar/AStarNode.h"
#include <cmath>

const double POSITIVE_INFINITY = std::numeric_limits<double>::max();

AStarNode::AStarNode(): MyCoordinate(), Parent(), StaticValue(POSITIVE_INFINITY),
    SumOfWeights(0), IsGenerated(false), IsExpanded(false), HeapPointer() {}

AStarNode::AStarNode(const Coordinate& coordinate):
        MyCoordinate(coordinate), Parent(), StaticValue(POSITIVE_INFINITY),
        SumOfWeights(0), IsGenerated(false), IsExpanded(false), HeapPointer() {}

AStarNode::AStarNode(const Coordinate& coordinate, const double f, const double g):
        MyCoordinate(coordinate), Parent(), StaticValue(f), SumOfWeights(g),
        IsGenerated(false), IsExpanded(false), HeapPointer(){}

AStarNode::AStarNode(const Coordinate& coordinate, const Coordinate& parent, const double f, const double g):
        MyCoordinate(coordinate), Parent(parent), StaticValue(f), SumOfWeights(g),
        IsGenerated(false), IsExpanded(false), HeapPointer() {}

AStarNode::AStarNode(const AStarNode& other):
        MyCoordinate(other.MyCoordinate), Parent(other.Parent), StaticValue(other.StaticValue),
        SumOfWeights(other.SumOfWeights), IsGenerated(other.IsGenerated),
        IsExpanded(other.IsExpanded), HeapPointer(){}

AStarNode::AStarNode(AStarNode&& other) noexcept: MyCoordinate(std::forward<Coordinate>(other.MyCoordinate)),
       Parent(std::forward<Coordinate>(other.Parent)), StaticValue(std::forward<double>(other.StaticValue)),
       SumOfWeights(std::forward<double>(other.SumOfWeights)), IsGenerated(std::forward<bool>(other.IsGenerated)),
       IsExpanded(std::forward<bool>(other.IsExpanded)),
       HeapPointer(std::forward<heap_t::handle_type>(other.HeapPointer)){}

std::ostream& operator << (std::ostream& out, const AStarNode& AStarNode)
{
    return out << '<' << AStarNode.MyCoordinate << ", f=" << AStarNode.StaticValue << ", g=" << AStarNode.SumOfWeights << '>';
}

AStarNode& AStarNode::operator=(AStarNode&& other) noexcept
{
    if(this != &other)
    {
        MyCoordinate = std::forward<Coordinate>(other.MyCoordinate);
        Parent = std::forward<Coordinate>(other.Parent);
        StaticValue = std::forward<double>(other.StaticValue);
        SumOfWeights = std::forward<double>(other.SumOfWeights);
        IsGenerated = std::forward<bool>(other.IsGenerated);
        IsExpanded = std::forward<bool>(other.IsExpanded);
        HeapPointer = std::forward<heap_t::handle_type>(other.HeapPointer);
    }
    return *this;
}

AStarNode& AStarNode::operator=(const AStarNode& other)
{
    if(this != &other)
    {
        MyCoordinate = other.MyCoordinate;
        Parent = other.Parent;
        StaticValue = other.StaticValue;
        SumOfWeights = other.SumOfWeights;
        IsGenerated = other.IsGenerated;
        IsExpanded = other.IsExpanded;
        HeapPointer = other.HeapPointer;
    }
    return *this;
}

bool AStarNode::operator==(const AStarNode &other) const
{
    return MyCoordinate == other.MyCoordinate;
}

bool AStarNode::operator!=(const AStarNode &other) const
{
    return !(*this == other);
}

bool AStarNodeComparator::operator()(const AStarNode *n1, const AStarNode *n2) const
{
    return (n1->StaticValue == n2->StaticValue) ? (n1->SumOfWeights > n2->SumOfWeights) :
           (n1->StaticValue > n2->StaticValue);
}