#include "../../include/RBFS/RbfsNode.h"
#include <cmath>
const double POSITIVE_INFINITY = std::numeric_limits<double>::max();

RbfsNode::RbfsNode():   MyCoordinate(), Parent(), StaticValue(POSITIVE_INFINITY),
                StoredValue(POSITIVE_INFINITY), SumOfWeights(0), Successors() {}

RbfsNode::RbfsNode(const Coordinate& coordinate):
    MyCoordinate(coordinate), Parent(), StaticValue(POSITIVE_INFINITY),
    StoredValue(POSITIVE_INFINITY), SumOfWeights(0),Successors() {}

RbfsNode::RbfsNode(const Coordinate& coordinate, const double f, const double F, const double g):
    MyCoordinate(coordinate), Parent(), StaticValue(f), StoredValue(F), SumOfWeights(g), Successors(){}

RbfsNode::RbfsNode(const Coordinate& coordinate, const Coordinate& parent, const double f, const double F, const double g):
        MyCoordinate(coordinate), Parent(parent), StaticValue(f), StoredValue(F), SumOfWeights(g),Successors() {}

RbfsNode::RbfsNode(const RbfsNode& other):
    MyCoordinate(other.MyCoordinate), Parent(other.Parent), StaticValue(other.StaticValue),
    StoredValue(other.StoredValue), SumOfWeights(other.SumOfWeights), Successors(other.Successors){}


RbfsNode::RbfsNode(RbfsNode&& other) noexcept: MyCoordinate(std::forward<Coordinate>(other.MyCoordinate)),
    Parent(std::forward<Coordinate>(other.Parent)), StaticValue(std::forward<double>(other.StaticValue)),
    StoredValue(std::forward<double>(other.StoredValue)),SumOfWeights(std::forward<double>(other.SumOfWeights)),
    Successors(std::forward<std::vector<RbfsNode*>>(other.Successors)){}

std::ostream& operator << (std::ostream& out, const RbfsNode& RbfsNode)
{
    return out << '<' << RbfsNode.MyCoordinate << ", f=" << RbfsNode.StaticValue << ", F=" << RbfsNode.StoredValue << ", g=" << RbfsNode.SumOfWeights << '>';
}

RbfsNode& RbfsNode::operator=(RbfsNode&& other) noexcept
{
    if(this != &other)
    {
        MyCoordinate = std::forward<Coordinate>(other.MyCoordinate);
        Parent = std::forward<Coordinate>(other.Parent);
        StaticValue = std::forward<double>(other.StaticValue);
        StoredValue = std::forward<double>(other.StoredValue);
        SumOfWeights = std::forward<double>(other.SumOfWeights);
        Successors = std::forward<std::vector<RbfsNode*>>(other.Successors);
    }
    return *this;
}

RbfsNode& RbfsNode::operator=(const RbfsNode& other)
{
    if(this != &other)
    {
        MyCoordinate = other.MyCoordinate;
        Parent = other.Parent;
        StaticValue = other.StaticValue;
        StoredValue = other.StoredValue;
        SumOfWeights = other.SumOfWeights;
        Successors = other.Successors;
    }
    return *this;
}

bool RbfsNode::operator==(const RbfsNode &other) const
{
    return MyCoordinate == other.MyCoordinate;
}

bool RbfsNode::operator!=(const RbfsNode &other) const
{
    return !(*this == other);
}

bool RbfsNode::RbfsNodeComparator(const RbfsNode* n1, const RbfsNode* n2)
{
    return (n1->StoredValue == n2->StoredValue) ? (n1->SumOfWeights < n2->SumOfWeights) :
    (n1->StoredValue < n2->StoredValue);
}