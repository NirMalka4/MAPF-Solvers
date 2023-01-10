#pragma once

#include "Map.h"
#include <vector>
#include "Agent.h"
#include "ISingleAgentPathFinder.h"

class Planner
{
private:
    Map CurrentMap;
    std::vector<std::vector<Agent>> Agents; // group agents by bucket
    ISingleAgentPathFinder* SingleAgentPathFinder;
    void LoadAgents(std::ifstream&, float const);
    float ParseVersion(std::ifstream&);
    bool ValidateVersion(float const);

public:
    Planner(const char* const, const char* const);
    virtual ~Planner() = default;

    const std::vector<std::vector<Agent>>& GetAgents(void) const;
    Map& GetMap(void);
    void SetAgentOnMap(Agent const&);
    void RemoveAgentFromMap(Agent const&);
    void SetSingleAgentPathFinder(ISingleAgentPathFinder*);
    Report Plan(const Agent&);
    void PlanAllScenarios(void);
};