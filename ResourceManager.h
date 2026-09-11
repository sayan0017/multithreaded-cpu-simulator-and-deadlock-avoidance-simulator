#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include "Process.h"

class ResourceManager {
public:
    ResourceManager(const std::vector<int>& total_resources);
    
    // Initializes banker's algorithm matrices for the given workload
    void initializeState(const std::vector<Process>& workload);

    // Checks if the current state is safe
    bool isSafeState();

    // Requests resources for a process. Returns true if granted (safe).
    bool requestResources(int pid, const std::vector<int>& request);

    // Releases resources for a completed process
    void releaseResources(int pid);

    // Returns a copy of the available resources
    std::vector<int> getAvailableResources() const;

private:
    std::vector<int> total_resources;
    std::vector<int> available;
    
    // Using pid as index (assuming pid starts from 1 and is contiguous for simplicity)
    // In a real system, we might use a hash map, but vectors are fine here.
    std::vector<std::vector<int>> max;
    std::vector<std::vector<int>> allocation;
    std::vector<std::vector<int>> need;
};

#endif // RESOURCE_MANAGER_H
