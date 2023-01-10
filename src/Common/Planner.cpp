#include "../../include/Common/Planner.h"
#include "../../include/Common/Printer.h"
#include <fstream> // ifstream

Planner::Planner(const char* const map_path, const char* const  scenario_path):
    CurrentMap(), Agents(), SingleAgentPathFinder(nullptr)
{
    std::ifstream scenario_file(scenario_path,std::ios::in);
    if(!scenario_file || !CurrentMap.Load(map_path))
    {
        DisplayMessage(Red,"Invalid paths for map or scenarios were supplied!\n");
        exit(EXIT_FAILURE);
    }
    float version = ParseVersion(scenario_file);
    LoadAgents(scenario_file, version);
    scenario_file.close();
}

inline bool Planner::ValidateVersion(float const version)
{
    return version == 0.0 || version == 1.0;
}

float Planner::ParseVersion(std::ifstream &scenario_file)
{
    /*
     * scenario_file contains rows in the following form:
     * ver = 1.0 : Bucket  map name  map width  map height  start x-coordinate  start y-coordinate  goal x-coordinate  goal y-coordinate  optimal length
     * ver = 0.0 : Bucket  map name start x-coordinate  start y-coordinate  goal x-coordinate  goal y-coordinate  optimal length
     */
    float version;
    std::string first;
    scenario_file >> first;

    // Check if a version number is given
    if (first != "version")
    {
        version = 0.0;
        scenario_file.seekg(0,std::ios::beg);
    }
    else
    {
        scenario_file >> version;
    }
    if(!ValidateVersion(version))
    {
        DisplayMessage(Red, "Invalid scenario version number is given.\nExpected either 1.0 or 0.0\n");
        exit(EXIT_FAILURE);
    }
    return version;
}

void Planner::LoadAgents(std::ifstream &scenario_file, float const version)
{
    constexpr size_t NUMBER_OF_BUCKETS = 128;
    Agents.resize(NUMBER_OF_BUCKETS);
    size_t agents_length = NUMBER_OF_BUCKETS;

    /*
     * scenario contains rows in the following form:
     * ver = 1.0 : Bucket  map name  map width  map height  start x-coordinate  start y-coordinate  goal x-coordinate  goal y-coordinate  optimal length
     * ver = 0.0 : Bucket  map name start x-coordinate  start y-coordinate  goal x-coordinate  goal y-coordinate  optimal length
     */

    std::string map_name;
    int map_width = 0, map_height = 0, start_x_coordinate, start_y_coordinate, goal_x_coordinate, goal_y_coordinate;
    double optimal_length;
    size_t bucket, max_bucket_number = 0;

    auto add_agent = [&]()
    {
        max_bucket_number = std::max(max_bucket_number, bucket);
        if(max_bucket_number > agents_length)
        {
            Agents.resize(max_bucket_number);
            agents_length = max_bucket_number;
        }
        Agents[bucket].emplace_back(start_x_coordinate, start_y_coordinate, goal_x_coordinate, goal_y_coordinate);
    };

    // Parse rows in their corresponding format, create agent from each
    if (version == 0.0)
    {
        while(scenario_file>>bucket>>map_name>>start_x_coordinate>>start_y_coordinate>>goal_x_coordinate>>goal_y_coordinate>>optimal_length)
        {
            add_agent();
        }
    }
    else
    {
        while(scenario_file>>bucket>>map_name>>map_width>>map_height>>start_x_coordinate>>start_y_coordinate>>goal_x_coordinate>>goal_y_coordinate>>optimal_length)
        {
            add_agent();
        }
    }
}

inline void Planner::SetAgentOnMap(Agent const& agent)
{
    CurrentMap.SetAgent(agent);
}

inline void Planner::RemoveAgentFromMap(Agent const& agent)
{
    CurrentMap.RemoveAgent(agent);
}

void Planner::SetSingleAgentPathFinder(ISingleAgentPathFinder* planner) 
{
    SingleAgentPathFinder = planner;
}

Report Planner::Plan(const Agent& agent)
{
    if(SingleAgentPathFinder == nullptr)
    {
        DisplayMessage(Red, "No single agent path finder is defined!\n");
        return {};
    }
    SingleAgentPathFinder->SetMap(&CurrentMap);

    SetAgentOnMap(agent);
    auto report =  SingleAgentPathFinder->SolveFullReport(agent);
    RemoveAgentFromMap(agent);
    return report;
}

void Planner::PlanAllScenarios(void)
{
    int bucket_number = 0, agent_number;
    int number_of_success_planning = 0, number_of_failed_planning = 0;
    constexpr int PATH = 0, AGENT = 1;

    for(auto const& agents : Agents)
    {
        agent_number = 0;
        for(auto const& agent : agents)
        {
            Report report = Plan(agent);
            if(get<0>(report).empty()) // no solution is found
            {
                number_of_failed_planning++;
            }
            else
            {
                number_of_success_planning++;
            }
            DisplayReport(report);
            DisplayMessage(CurrentMap.GetGridWithSolution(get<PATH>(report), get<AGENT>(report)));
            agent_number++;
        }
        bucket_number++;
    }
    DisplayMessage(Blue, "Succeeded planning: ", number_of_success_planning, " Failed planning: ", number_of_failed_planning);
}

const std::vector<std::vector<Agent>>& Planner::GetAgents(void) const
{
    return Agents;
}

Map& Planner::GetMap(void)
{
    return CurrentMap;
}