#pragma once

#include "../Common/Coordinate.h"
#include <boost/heap/binomial_heap.hpp>

class PEAStarNode
{
public:
    Coordinate MyCoordinate, Parent;
    double StaticValue; // f-value, estimated cost of the cheapest path (root..PEAStarNode..goal)
    double StoredValue; // F-value, least f-value of successors, that was propagated to this RbfsNode by a collapse action.
    double SumOfWeights; // g-value, the cheapest sum of weights to reach this PEAStarNode
    bool IsExpanded; // whether this PEAStarNode is already being popped from open set (=belongs to close-set)
    bool IsOpen;// whether this PEAStarNode has been inserted to open-set

    PEAStarNode();
    PEAStarNode(const Coordinate&);
    PEAStarNode(const Coordinate&, const double, const double, const double);
    PEAStarNode(const Coordinate&, const Coordinate&, const double, const double, const double);
    PEAStarNode(const PEAStarNode&);
    PEAStarNode(PEAStarNode&&) noexcept ;
    virtual ~PEAStarNode() = default;

    friend std::ostream& operator<<(std::ostream&, const PEAStarNode&);
    PEAStarNode& operator = (const PEAStarNode&);
    PEAStarNode& operator = (PEAStarNode&&) noexcept ;
    bool operator == (const PEAStarNode&) const;
    bool operator != (const PEAStarNode&) const;
};