#include "SimulationManager.h"
#include <iostream>
#include <iomanip>

SimulationManager::SimulationManager(const std::vector<Process>& workload, const std::vector<int>& resources)
    : base_workload(workload), total_resources(resources) {}

void SimulationManager::addScheduler(std::shared_ptr<Scheduler> scheduler) {
    schedulers.push_back(scheduler);
}

void SimulationManager::runSingleScheduler(std::shared_ptr<Scheduler> scheduler) {
    // Each thread gets its own copy of the workload and a fresh ResourceManager
    std::vector<Process> local_workload = base_workload;
    ResourceManager local_rm(total_resources);
    local_rm.initializeState(local_workload);
    
    std::cout << "[SIM] Starting " << scheduler->getName() << "..." << std::endl;
    
    // Run simulation
    scheduler->simulate(local_workload, local_rm);
    
    // Calculate metrics
    double total_wait = 0, total_turnaround = 0, total_response = 0;
    int max_completion_time = 0;
    int total_burst = 0;
    
    for (const auto& p : local_workload) {
        total_wait += p.waiting_time;
        total_turnaround += p.turnaround_time;
        total_response += p.response_time;
        total_burst += p.burst_time;
        if (p.completion_time > max_completion_time) {
            max_completion_time = p.completion_time;
        }
    }
    
    int n = local_workload.size();
    Metrics m;
    m.scheduler_name = scheduler->getName();
    m.avg_waiting_time = total_wait / n;
    m.avg_turnaround_time = total_turnaround / n;
    m.avg_response_time = total_response / n;
    
    if (max_completion_time > 0) {
        m.cpu_utilization = (double)total_burst / max_completion_time * 100.0;
    } else {
        m.cpu_utilization = 0;
    }
    
    results.push_back(m);
    
    std::cout << "[SIM] Finished " << scheduler->getName() << std::endl;
}

void SimulationManager::runSimulation() {
    for (auto& sched : schedulers) {
        runSingleScheduler(sched);
    }
}

void SimulationManager::printReport() {
    
    std::cout << "\n========================================================================================\n";
    std::cout << "                                  SIMULATION REPORT                                     \n";
    std::cout << "========================================================================================\n";
    
    std::cout << std::left 
              << std::setw(25) << "Algorithm" 
              << std::setw(15) << "Avg Wait Time" 
              << std::setw(20) << "Avg Turnaround Time" 
              << std::setw(15) << "Avg Resp Time" 
              << std::setw(15) << "CPU Util (%)" << "\n";
              
    std::cout << "----------------------------------------------------------------------------------------\n";
    
    for (const auto& r : results) {
        std::cout << std::left 
                  << std::setw(25) << r.scheduler_name
                  << std::setw(15) << std::fixed << std::setprecision(2) << r.avg_waiting_time
                  << std::setw(20) << r.avg_turnaround_time
                  << std::setw(15) << r.avg_response_time
                  << std::setw(15) << r.cpu_utilization << "\n";
    }
    std::cout << "========================================================================================\n";
}
