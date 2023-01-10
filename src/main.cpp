#include "../include/Common/Planner.h"
#include <cstdlib> // exit
#include "../include/AStar/AStar.h"
#include "../include/RBFS/RBFS.h"
#include "../include/PEAStar/PEAStar.h"
#include "../include/Common/Printer.h"

void RunPEAStar(Planner&);
void RunAStar(Planner&);
void RunRBFS(Planner&);
void CompareAStarToRbfs(Planner&);

int main(int argc, char** const argv)
{
    if(argc != 3)
    {
        DisplayMessage(Red, "Expected 2 arguments: path to map file and path to scenario file!\n");
        exit(EXIT_FAILURE);
    }

    Planner planner(argv[1], argv[2]);
    CompareAStarToRbfs(planner);
    exit(EXIT_SUCCESS);
}

void RunRBFS(Planner& planner)
{
    RBFS rbfs(Manhattan);
    planner.SetSingleAgentPathFinder(&rbfs);
    planner.PlanAllScenarios();
}

void RunPEAStar(Planner& planner)
{
    PEAStar peastar(Manhattan);
    planner.SetSingleAgentPathFinder(&peastar);
    planner.PlanAllScenarios();
}

void RunAStar(Planner& planner)
{
    AStar astar(Manhattan);
    planner.SetSingleAgentPathFinder(&astar);
    planner.PlanAllScenarios();
}

void CompareAStarToRbfs(Planner& planner)
{
    AStar astar(Manhattan);
    RBFS rbfs(Manhattan);
    constexpr int PATH = 0, AGENT = 1;

    size_t correct_answer = 0, wrong_answer = 0;
    for(const auto& agents : planner.GetAgents())
    {
        for(const auto& agent : agents)
        {
            planner.SetSingleAgentPathFinder(&astar);
            auto astar_report = planner.Plan(agent);

            planner.SetSingleAgentPathFinder(&rbfs);
            auto rbfs_report = planner.Plan(agent);

            if(get<PATH>(astar_report).size() != get<PATH>(rbfs_report).size())
            {
                Map& map = planner.GetMap();
                wrong_answer++;
                DisplayMessage(Red, "Failure!!!!\nA* report is: \n");
                DisplayReport(astar_report);
                DisplayMessage(map.GetGridWithSolution(get<PATH>(astar_report), get<AGENT>(astar_report)));

                DisplayMessage(Red, "RBFS report is: \n");
                DisplayReport(rbfs_report);
                DisplayMessage(map.GetGridWithSolution(get<PATH>(rbfs_report), get<AGENT>(astar_report)));
            }
            else
            {
                correct_answer++;
            }
        }
    }

    DisplayMessage(Green, "Correct answers = ", correct_answer, " Wrong answer = ", wrong_answer, '\n');
}