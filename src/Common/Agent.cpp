#include "../../include/Common/Agent.h"

Agent::Agent() : Start(), Goal() {}
Agent::Agent(int const start_row, int const start_column, int const goal_row, int const goal_column) : Start(start_row, start_column), Goal(goal_row, goal_column) {}
Agent::Agent(Agent const& other): Start(other.Start), Goal(other.Goal) {}
Agent::Agent(Coordinate const& start, Coordinate const& goal): Start(start), Goal(goal) {}
Agent::Agent(Coordinate&& start, Coordinate&& goal):
        Start(std::forward<Coordinate>(start)), Goal(std::forward<Coordinate>(goal)) {}
Agent::Agent(Agent&& other) noexcept: Start(std::forward<Coordinate>(other.Start)), Goal(std::forward<Coordinate>(other.Goal)) {}

Agent& Agent::operator=(Agent const& other)
{
    if(this != &other)
    {
        Start = other.Start;
        Goal = other.Goal;
    }
    return *this;
}
Agent& Agent::operator=(Agent &&other) noexcept
{
    if(this != &other)
    {
        Start = std::forward<Coordinate>(other.Start);
        Goal = std::forward<Coordinate>(other.Goal);
    }
    return *this;
}

Coordinate Agent::GetStartCoordinate() const {return Start;}
Coordinate Agent::GetGoalCoordinate() const {return Goal;}