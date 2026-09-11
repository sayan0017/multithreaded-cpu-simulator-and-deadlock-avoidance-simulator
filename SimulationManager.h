#ifndef SIMULATION_MANAGER_H
#define SIMULATION_MANAGER_H

#include <vector>
#include <memory>
#include "Schedulers.h"
#include "ResourceManager.h"
#include "Process.h"

class SimulationManager {
public:
    SimulationManager(const std::vector<Process>& base_workload, const std::vector<int>& total_resources);
    
    // Register a scheduler for the simulation
    void addScheduler(std::shared_ptr<Scheduler> scheduler);
    
    // Run all registered schedulers concurrently
    void runSimulation();
    
    // Print the final metric report
    void printReport();

private:
    std::vector<Process> base_workload;
    std::vector<int> total_resources;
    std::vector<std::shared_ptr<Scheduler>> schedulers;
    
    struct Metrics {
        std::string scheduler_name;
        double avg_waiting_time;
        double avg_turnaround_time;
        double avg_response_time;
        double cpu_utilization;
    };
    
    std::vector<Metrics> results;
    
    void runSingleScheduler(std::shared_ptr<Scheduler> scheduler);
};

#endif // SIMULATION_MANAGER_H
