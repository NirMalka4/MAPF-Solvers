#pragma once
#include "Coordinate.h"

class Agent
{
private:
    Coordinate Start, Goal;

public:
    Agent();
    Agent(int const, int const, int const, int const);
    Agent(Coordinate&&, Coordinate&&);
    Agent(Coordinate const&, Coordinate const&);
    Agent(Agent const&);
    Agent(Agent&&) noexcept ;

    Agent& operator = (Agent const&);
    Agent& operator = (Agent&&) noexcept ;

    Coordinate GetStartCoordinate(void) const;
    Coordinate GetGoalCoordinate(void) const;
};