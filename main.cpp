#include <iostream>
#include <vector>
#include <memory>
#include "Process.h"
#include "Schedulers.h"
#include "SimulationManager.h"

int main() {
    std::cout << "Initializing CPU Scheduler & Deadlock Avoidance Simulator...\n";

    // Define standard available resources for the system
    // Let's assume 3 resource types: A (10 units), B (5 units), C (7 units)
    std::vector<int> total_resources = {10, 5, 7};

    // Create a workload of processes
    // Process(pid, arrival_time, burst_time, priority, allocated, max)
    std::vector<Process> workload = {
        Process(1, 0, 8, 2, {0, 1, 0}, {7, 5, 3}),
        Process(2, 1, 4, 1, {2, 0, 0}, {3, 2, 2}),
        Process(3, 2, 9, 4, {3, 0, 2}, {9, 0, 2}),
        Process(4, 3, 5, 3, {2, 1, 1}, {2, 2, 2}),
        Process(5, 4, 2, 5, {0, 0, 2}, {4, 3, 3})
    };

    // Instantiate simulation manager
    SimulationManager sim(workload, total_resources);

    // Register scheduling algorithms
    sim.addScheduler(std::make_shared<FCFS>());
    sim.addScheduler(std::make_shared<SJF>());
    sim.addScheduler(std::make_shared<PriorityScheduler>());
    sim.addScheduler(std::make_shared<RoundRobin>(3)); // Time quantum = 3
    sim.addScheduler(std::make_shared<MLFQ>());

    // Run the multithreaded simulation
    sim.runSimulation();

    // Print benchmark metrics
    sim.printReport();

    return 0;
}
