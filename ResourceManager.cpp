#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager(const std::vector<int>& total) 
    : total_resources(total), available(total) {
}

void ResourceManager::initializeState(const std::vector<Process>& workload) {
    available = total_resources;
    
    // Determine the max PID to size our vectors
    int max_pid = 0;
    for (const auto& p : workload) {
        if (p.pid > max_pid) max_pid = p.pid;
    }
    
    // We add 1 because PIDs are 1-based in our example
    max.assign(max_pid + 1, std::vector<int>(total_resources.size(), 0));
    allocation.assign(max_pid + 1, std::vector<int>(total_resources.size(), 0));
    need.assign(max_pid + 1, std::vector<int>(total_resources.size(), 0));
    
    for (const auto& p : workload) {
        max[p.pid] = p.max_resources;
        allocation[p.pid] = p.allocated_resources;
        need[p.pid] = p.need_resources;
        
        // Subtract allocated resources from available
        for (size_t i = 0; i < total_resources.size(); ++i) {
            available[i] -= p.allocated_resources[i];
        }
    }
}

bool ResourceManager::isSafeState() {
    int n = allocation.size(); // number of process slots
    int m = available.size();  // number of resources
    
    std::vector<int> work = available;
    std::vector<bool> finish(n, false);
    
    // Mark processes with 0 allocation as finished to skip them
    // Also skip pid 0 if we are 1-indexing
    for (int i = 0; i < n; ++i) {
        bool has_alloc = false;
        for (int j = 0; j < m; ++j) {
            if (allocation[i][j] > 0) has_alloc = true;
        }
        if (!has_alloc && i != 0) { // If it has max but 0 alloc, it still needs to be checked, but for simplicity...
            // Actually, we should check if max is 0.
            bool has_max = false;
            for (int j = 0; j < m; ++j) {
                if (max[i][j] > 0) has_max = true;
            }
            if (!has_max) finish[i] = true;
        }
    }
    finish[0] = true; // Skip index 0
    
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (finish[i]) count++;
    }

    while (count < n) {
        bool found = false;
        for (int i = 1; i < n; ++i) {
            if (!finish[i]) {
                bool can_satisfy = true;
                for (int j = 0; j < m; ++j) {
                    if (need[i][j] > work[j]) {
                        can_satisfy = false;
                        break;
                    }
                }
                
                if (can_satisfy) {
                    for (int j = 0; j < m; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) {
            return false; // Not safe
        }
    }
    return true; // Safe
}

bool ResourceManager::requestResources(int pid, const std::vector<int>& request) {
    int m = available.size();
    
    // 1. Check if request <= need
    for (int i = 0; i < m; ++i) {
        if (request[i] > need[pid][i]) {
            return false; // Error condition, requested more than needed
        }
    }
    
    // 2. Check if request <= available
    for (int i = 0; i < m; ++i) {
        if (request[i] > available[i]) {
            return false; // Must wait
        }
    }
    
    // 3. Pretend to allocate
    for (int i = 0; i < m; ++i) {
        available[i] -= request[i];
        allocation[pid][i] += request[i];
        need[pid][i] -= request[i];
    }
    
    // 4. Check safety
    if (isSafeState()) {
        return true; // Safe, resources allocated
    } else {
        // Rollback
        for (int i = 0; i < m; ++i) {
            available[i] += request[i];
            allocation[pid][i] -= request[i];
            need[pid][i] += request[i];
        }
        return false; // Unsafe, revert and block
    }
}

void ResourceManager::releaseResources(int pid) {
    int m = available.size();
    for (int i = 0; i < m; ++i) {
        available[i] += allocation[pid][i];
        allocation[pid][i] = 0;
        need[pid][i] = max[pid][i]; // Reset need if we want to reuse process, otherwise it's 0.
    }
}
