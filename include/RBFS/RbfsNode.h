#pragma once
#include "../Common/Coordinate.h"
#include <vector>

class RbfsNode
{
public:
    Coordinate MyCoordinate, Parent;
    double StaticValue; // f-value, estimated cost of the cheapest path (root..RbfsNode..goal)
    double StoredValue; // F-value, least f-value of successors, that was propagated to this RbfsNode by a collapse action.
    double SumOfWeights; // g-value, the cheapest sum of weights to reach this RbfsNode
    std::vector<RbfsNode*> Successors;

    RbfsNode();
    RbfsNode(const Coordinate&);
    RbfsNode(const Coordinate&, const double, const double, const double);
    RbfsNode(const Coordinate&, const Coordinate&, const double, const double, const double);
    RbfsNode(const RbfsNode&);
    RbfsNode(RbfsNode&&) noexcept ;
    virtual ~RbfsNode() = default;

    friend std::ostream& operator<<(std::ostream&, const RbfsNode&);
    RbfsNode& operator = (const RbfsNode&);
    RbfsNode& operator = (RbfsNode&&) noexcept ;
    bool operator == (const RbfsNode&) const;
    bool operator != (const RbfsNode&) const;

    static bool RbfsNodeComparator(const RbfsNode*, const RbfsNode*);
};