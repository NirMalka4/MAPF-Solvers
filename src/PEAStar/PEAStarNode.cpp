#include "../../include/PEAStar/PEAStarNode.h"
#include <cmath>

const double POSITIVE_INFINITY = std::numeric_limits<double>::max();

PEAStarNode::PEAStarNode(): MyCoordinate(), Parent(), StaticValue(POSITIVE_INFINITY),
                        StoredValue(POSITIVE_INFINITY), SumOfWeights(0), IsExpanded(false), IsOpen(false) {}

PEAStarNode::PEAStarNode(const Coordinate& coordinate):
        MyCoordinate(coordinate), Parent(), StaticValue(POSITIVE_INFINITY),
        StoredValue(POSITIVE_INFINITY), SumOfWeights(0), IsExpanded(false), IsOpen(false) {}

PEAStarNode::PEAStarNode(const Coordinate& coordinate, const double f, const double F, const double g):
        MyCoordinate(coordinate), Parent(), StaticValue(f), StoredValue(F),
        SumOfWeights(g), IsExpanded(false){}

PEAStarNode::PEAStarNode(const Coordinate& coordinate, const Coordinate& parent, const double f, const double F, const double g):
        MyCoordinate(coordinate), Parent(parent), StaticValue(f),
        StoredValue(F), SumOfWeights(g), IsExpanded(false), IsOpen(false){}

PEAStarNode::PEAStarNode(const PEAStarNode& other):
        MyCoordinate(other.MyCoordinate), Parent(other.Parent), StaticValue(other.StaticValue),
        StoredValue(other.StoredValue), SumOfWeights(other.SumOfWeights), IsExpanded(other.IsExpanded),
        IsOpen(other.IsOpen){}

PEAStarNode::PEAStarNode(PEAStarNode&& other) noexcept:
    MyCoordinate(std::forward<Coordinate>(other.MyCoordinate)),
    Parent(std::forward<Coordinate>(other.Parent)),
    StaticValue(std::forward<double>(other.StaticValue)),
    StoredValue(std::forward<double>(other.StoredValue)),
    SumOfWeights(std::forward<double>(other.SumOfWeights)),
    IsExpanded(std::forward<bool>(other.IsExpanded)),
    IsOpen(std::forward<bool>(other.IsOpen)){}

std::ostream& operator << (std::ostream& out, const PEAStarNode& node)
{
    return out << '<' << node.MyCoordinate << ", f=" << node.StaticValue << ", F=" << node.StoredValue <<
    ", g=" << node.SumOfWeights << '>';
}

PEAStarNode& PEAStarNode::operator=(PEAStarNode&& other) noexcept
{
    if(this != &other)
    {
        MyCoordinate = std::forward<Coordinate>(other.MyCoordinate);
        Parent = std::forward<Coordinate>(other.Parent);
        StaticValue = std::forward<double>(other.StaticValue);
        StoredValue = std::forward<double>(other.StoredValue);
        SumOfWeights = std::forward<double>(other.SumOfWeights);
        IsExpanded = std::forward<bool>(other.IsExpanded);
        IsOpen = std::forward<bool>(other.IsOpen);
    }
    return *this;
}

PEAStarNode& PEAStarNode::operator=(const PEAStarNode& other)
{
    if(this != &other)
    {
        MyCoordinate = other.MyCoordinate;
        Parent = other.Parent;
        StaticValue = other.StaticValue;
        StoredValue = other.StoredValue;
        SumOfWeights = other.SumOfWeights;
        IsExpanded = other.IsExpanded;
        IsOpen = other.IsOpen;
    }
    return *this;
}

bool PEAStarNode::operator==(const PEAStarNode &other) const
{
    return MyCoordinate == other.MyCoordinate;
}

bool PEAStarNode::operator!=(const PEAStarNode &other) const
{
    return !(*this == other);
}