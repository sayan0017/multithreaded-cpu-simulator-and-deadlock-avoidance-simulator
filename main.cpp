#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "Process.h"
#include "Schedulers.h"
#include "SimulationManager.h"

bool readInteger(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;

        if (!(std::cin >> value)) {
            if (std::cin.eof()) {
                return false;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter an integer.\n";
            continue;
        }

        std::string remainder;
        std::getline(std::cin, remainder);
        std::istringstream remainderStream(remainder);
        std::string extraInput;
        if (remainderStream >> extraInput) {
            std::cout << "Invalid input. Please enter an integer.\n";
            continue;
        }

        return true;
    }
}

bool readPositiveInteger(const std::string& prompt, int& value) {
    while (readInteger(prompt, value)) {
        if (value > 0) {
            return true;
        }

        std::cout << "Value must be greater than 0. Please try again.\n";
    }

    return false;
}

bool readNonNegativeInteger(const std::string& prompt, int& value) {
    while (readInteger(prompt, value)) {
        if (value >= 0) {
            return true;
        }

        std::cout << "Value cannot be negative. Please try again.\n";
    }

    return false;
}

int main() {
    std::cout << "Initializing CPU Scheduler & Deadlock Avoidance Simulator...\n";

    // Define standard available resources for the system
    // Let's assume 3 resource types: A (10 units), B (5 units), C (7 units)
    std::vector<int> total_resources = {10, 5, 7};

    int process_count;
    if (!readPositiveInteger("Enter the number of processes: ", process_count)) {
        return 1;
    }

    std::vector<Process> workload;
    workload.reserve(process_count);

    // Validate each process before adding it to the simulation workload.
    for (int process_index = 0; process_index < process_count; ++process_index) {
        int arrival_time;
        int burst_time;
        int priority;
        std::vector<int> allocated_resources(total_resources.size());
        std::vector<int> max_resources(total_resources.size());

        std::cout << "\nEnter parameters for process " << process_index + 1 << ":\n";
        if (!readNonNegativeInteger("Arrival time: ", arrival_time) ||
            !readPositiveInteger("Burst time: ", burst_time) ||
            !readNonNegativeInteger("Priority: ", priority)) {
            return 1;
        }

        for (size_t resource_index = 0; resource_index < total_resources.size(); ++resource_index) {
            const std::string resource_name = "Maximum resource " + std::to_string(resource_index + 1) + ": ";
            if (!readNonNegativeInteger(resource_name, max_resources[resource_index])) {
                return 1;
            }
        }

        for (size_t resource_index = 0; resource_index < total_resources.size(); ++resource_index) {
            const std::string resource_name = "Allocated resource " + std::to_string(resource_index + 1) + ": ";
            while (true) {
                if (!readNonNegativeInteger(resource_name, allocated_resources[resource_index])) {
                    return 1;
                }

                if (allocated_resources[resource_index] <= max_resources[resource_index]) {
                    break;
                }

                std::cout << "Allocated resource cannot exceed the maximum requirement ("
                          << max_resources[resource_index] << "). Please try again.\n";
            }
        }

        workload.emplace_back(process_index + 1, arrival_time, burst_time, priority,
                              allocated_resources, max_resources);
    }

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
