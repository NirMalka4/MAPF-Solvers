#pragma once

#include "../Common/Coordinate.h"
#include <boost/heap/fibonacci_heap.hpp>

class AStarNode;
struct AStarNodeComparator
{
    bool operator() (const AStarNode*, const AStarNode*) const;
};
using heap_t = boost::heap::fibonacci_heap<AStarNode*, boost::heap::compare<AStarNodeComparator>>;

class AStarNode
{
public:
    Coordinate MyCoordinate, Parent;
    double StaticValue; // f-value, estimated cost of the cheapest path (root..AStarNode..goal)
    double SumOfWeights; // g-value, the cheapest sum of weights to reach this AStarNode
    bool IsGenerated; // whether this AStarNode has been created, inserted to open-set but not yet expanded
    bool IsExpanded; // whether this AStarNode is already being popped from open set (=belongs to close-set)
    heap_t::handle_type HeapPointer; // pointer to AStarNode in open-set. used for decrease key operation.

    AStarNode();
    AStarNode(const Coordinate&);
    AStarNode(const Coordinate&, const double, const double);
    AStarNode(const Coordinate&, const Coordinate&, const double, const double);
    AStarNode(const AStarNode&);
    AStarNode(AStarNode&&) noexcept ;
    virtual ~AStarNode() = default;

    friend std::ostream& operator<<(std::ostream&, const AStarNode&);
    AStarNode& operator = (const AStarNode&);
    AStarNode& operator = (AStarNode&&) noexcept ;
    bool operator == (const AStarNode&) const;
    bool operator != (const AStarNode&) const;
};