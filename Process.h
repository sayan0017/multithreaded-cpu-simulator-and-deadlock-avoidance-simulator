#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <cstddef>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    
    // Metrics
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    
    // Resource Management (Banker's Algorithm)
    std::vector<int> allocated_resources;
    std::vector<int> max_resources;
    std::vector<int> need_resources;

    // For queueing
    int current_queue_level; // Used in MLFQ

    Process(int id, int arr, int burst, int prio, 
            const std::vector<int>& allocated, 
            const std::vector<int>& max)
        : pid(id), arrival_time(arr), burst_time(burst), remaining_time(burst), priority(prio),
          start_time(-1), completion_time(0), waiting_time(0), turnaround_time(0), response_time(-1),
          allocated_resources(allocated), max_resources(max), current_queue_level(0) {
        
        need_resources.resize(max_resources.size());
        for (size_t i = 0; i < max_resources.size(); ++i) {
            need_resources[i] = max_resources[i] - allocated_resources[i];
        }
    }
};

#endif // PROCESS_H
