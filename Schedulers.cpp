#include "Schedulers.h"
#include <algorithm>
#include <queue>
#include <iostream>

Scheduler::Scheduler(const std::string& name) : name(name) {}

std::string Scheduler::getName() const {
    return name;
}

bool Scheduler::requestFullNeed(Process& p, ResourceManager& rm) {
    bool has_need = false;
    for (int need_amt : p.need_resources) {
        if (need_amt > 0) has_need = true;
    }
    
    if (has_need) {
        if (rm.requestResources(p.pid, p.need_resources)) {
            // Fill allocated
            for (size_t i = 0; i < p.need_resources.size(); ++i) {
                p.allocated_resources[i] += p.need_resources[i];
                p.need_resources[i] = 0;
            }
            return true;
        }
        return false;
    }
    return true; // No need
}

// ---------------- FCFS ----------------
void FCFS::simulate(std::vector<Process>& workload, ResourceManager& rm) {
    int current_time = 0;
    int completed = 0;
    int n = workload.size();
    
    std::sort(workload.begin(), workload.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    while (completed < n) {
        bool process_ran = false;
        
        for (auto& p : workload) {
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                if (requestFullNeed(p, rm)) {
                    if (p.start_time == -1) p.start_time = current_time;
                    
                    current_time += p.remaining_time;
                    p.remaining_time = 0;
                    p.completion_time = current_time;
                    p.turnaround_time = p.completion_time - p.arrival_time;
                    p.waiting_time = p.turnaround_time - p.burst_time;
                    p.response_time = p.start_time - p.arrival_time;
                    
                    rm.releaseResources(p.pid);
                    completed++;
                    process_ran = true;
                    break;
                }
            }
        }
        if (!process_ran) current_time++;
    }
}

// ---------------- SJF (SRTF) ----------------
void SJF::simulate(std::vector<Process>& workload, ResourceManager& rm) {
    int current_time = 0;
    int completed = 0;
    int n = workload.size();
    
    while (completed < n) {
        std::vector<int> ready;
        for (int i = 0; i < n; ++i) {
            if (workload[i].arrival_time <= current_time && workload[i].remaining_time > 0) {
                ready.push_back(i);
            }
        }
        
        std::sort(ready.begin(), ready.end(), [&](int a, int b) {
            return workload[a].remaining_time < workload[b].remaining_time;
        });
        
        bool process_ran = false;
        for (int idx : ready) {
            auto& p = workload[idx];
            if (requestFullNeed(p, rm)) {
                if (p.start_time == -1) p.start_time = current_time;
                current_time++;
                p.remaining_time--;
                
                if (p.remaining_time == 0) {
                    p.completion_time = current_time;
                    p.turnaround_time = p.completion_time - p.arrival_time;
                    p.waiting_time = p.turnaround_time - p.burst_time;
                    p.response_time = p.start_time - p.arrival_time;
                    rm.releaseResources(p.pid);
                    completed++;
                }
                process_ran = true;
                break;
            }
        }
        if (!process_ran) current_time++;
    }
}

// ---------------- Priority (Preemptive) ----------------
void PriorityScheduler::simulate(std::vector<Process>& workload, ResourceManager& rm) {
    int current_time = 0;
    int completed = 0;
    int n = workload.size();
    
    while (completed < n) {
        std::vector<int> ready;
        for (int i = 0; i < n; ++i) {
            if (workload[i].arrival_time <= current_time && workload[i].remaining_time > 0) {
                ready.push_back(i);
            }
        }
        
        std::sort(ready.begin(), ready.end(), [&](int a, int b) {
            return workload[a].priority > workload[b].priority;
        });
        
        bool process_ran = false;
        for (int idx : ready) {
            auto& p = workload[idx];
            if (requestFullNeed(p, rm)) {
                if (p.start_time == -1) p.start_time = current_time;
                current_time++;
                p.remaining_time--;
                
                if (p.remaining_time == 0) {
                    p.completion_time = current_time;
                    p.turnaround_time = p.completion_time - p.arrival_time;
                    p.waiting_time = p.turnaround_time - p.burst_time;
                    p.response_time = p.start_time - p.arrival_time;
                    rm.releaseResources(p.pid);
                    completed++;
                }
                process_ran = true;
                break;
            }
        }
        if (!process_ran) current_time++;
    }
}

// ---------------- Round Robin ----------------
void RoundRobin::simulate(std::vector<Process>& workload, ResourceManager& rm) {
    int current_time = 0;
    int completed = 0;
    int n = workload.size();
    
    std::queue<int> ready_queue;
    std::vector<bool> in_queue(n, false);
    
    auto enqueue_new = [&](int time) {
        for (int i = 0; i < n; ++i) {
            if (workload[i].arrival_time <= time && workload[i].remaining_time > 0 && !in_queue[i]) {
                ready_queue.push(i);
                in_queue[i] = true;
            }
        }
    };

    enqueue_new(current_time);

    while (completed < n) {
        if (ready_queue.empty()) {
            current_time++;
            enqueue_new(current_time);
            continue;
        }

        int attempts = ready_queue.size();
        bool process_ran = false;
        
        while (attempts > 0 && !process_ran) {
            int idx = ready_queue.front();
            ready_queue.pop();
            attempts--;
            
            auto& p = workload[idx];
            
            if (requestFullNeed(p, rm)) {
                if (p.start_time == -1) p.start_time = current_time;
                
                int time_slice = std::min(quantum, p.remaining_time);
                current_time += time_slice;
                p.remaining_time -= time_slice;
                
                enqueue_new(current_time); // Enqueue newly arrived
                
                if (p.remaining_time == 0) {
                    p.completion_time = current_time;
                    p.turnaround_time = p.completion_time - p.arrival_time;
                    p.waiting_time = p.turnaround_time - p.burst_time;
                    p.response_time = p.start_time - p.arrival_time;
                    rm.releaseResources(p.pid);
                    completed++;
                    in_queue[idx] = false;
                } else {
                    ready_queue.push(idx); // put back at end
                    // still in_queue is true
                }
                process_ran = true;
            } else {
                // Unsafe, put it at back and try next
                ready_queue.push(idx);
            }
        }
        
        if (!process_ran) {
            current_time++;
            enqueue_new(current_time);
        }
    }
}

// ---------------- MLFQ (3 Queues) ----------------
void MLFQ::simulate(std::vector<Process>& workload, ResourceManager& rm) {
    int current_time = 0;
    int completed = 0;
    int n = workload.size();
    
    std::queue<int> q0, q1, q2;
    std::vector<bool> in_queue(n, false);
    
    auto enqueue_new = [&](int time) {
        for (int i = 0; i < n; ++i) {
            if (workload[i].arrival_time <= time && workload[i].remaining_time > 0 && !in_queue[i]) {
                q0.push(i);
                workload[i].current_queue_level = 0;
                in_queue[i] = true;
            }
        }
    };
    
    while (completed < n) {
        enqueue_new(current_time);
        
        bool process_ran = false;
        
        auto try_queue = [&](std::queue<int>& q, int time_slice, int queue_used) {
            int attempts = q.size();
            while (attempts > 0 && !process_ran) {
                int active_idx = q.front();
                q.pop();
                attempts--;
                
                auto& p = workload[active_idx];
                
                if (requestFullNeed(p, rm)) {
                    if (p.start_time == -1) p.start_time = current_time;
                    
                    int run_time = std::min(time_slice, p.remaining_time);
                    current_time += run_time;
                    p.remaining_time -= run_time;
                    
                    enqueue_new(current_time);
                    
                    if (p.remaining_time == 0) {
                        p.completion_time = current_time;
                        p.turnaround_time = p.completion_time - p.arrival_time;
                        p.waiting_time = p.turnaround_time - p.burst_time;
                        p.response_time = p.start_time - p.arrival_time;
                        rm.releaseResources(p.pid);
                        completed++;
                        in_queue[active_idx] = false;
                    } else {
                        // Demote
                        if (queue_used == 0) {
                            p.current_queue_level = 1;
                            q1.push(active_idx);
                        } else {
                            p.current_queue_level = 2;
                            q2.push(active_idx);
                        }
                    }
                    process_ran = true;
                } else {
                    q.push(active_idx);
                }
            }
        };
        
        if (!process_ran && !q0.empty()) try_queue(q0, 4, 0);
        if (!process_ran && !q1.empty()) try_queue(q1, 8, 1);
        if (!process_ran && !q2.empty()) {
            // Q2 is FCFS, but let's just let it run to completion or run_time = remaining_time
            int active_idx = q2.front();
            int time_slice = workload[active_idx].remaining_time;
            try_queue(q2, time_slice, 2);
        }
        
        if (!process_ran) {
            current_time++;
        }
    }
}
