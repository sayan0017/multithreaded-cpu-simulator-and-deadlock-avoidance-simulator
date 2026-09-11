#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include <vector>
#include <string>
#include "Process.h"
#include "ResourceManager.h"

class Scheduler {
public:
    Scheduler(const std::string& name);
    virtual ~Scheduler() = default;

    virtual void simulate(std::vector<Process>& workload, ResourceManager& rm) = 0;

    std::string getName() const;

protected:
    std::string name;
    
    void calculateMetrics(std::vector<Process>& workload);
    void printProcessTimeline(const std::string& msg);
    
    // Check if process can get its need, for simplicity we might just request its entire need
    bool requestFullNeed(Process& p, ResourceManager& rm);
};

class FCFS : public Scheduler {
public:
    FCFS() : Scheduler("FCFS") {}
    void simulate(std::vector<Process>& workload, ResourceManager& rm) override;
};

class SJF : public Scheduler {
public:
    SJF() : Scheduler("SJF (SRTF)") {}
    void simulate(std::vector<Process>& workload, ResourceManager& rm) override;
};

class PriorityScheduler : public Scheduler {
public:
    PriorityScheduler() : Scheduler("Priority (Preemptive)") {}
    void simulate(std::vector<Process>& workload, ResourceManager& rm) override;
};

class RoundRobin : public Scheduler {
public:
    RoundRobin(int quantum) : Scheduler("Round Robin (q=" + std::to_string(quantum) + ")"), quantum(quantum) {}
    void simulate(std::vector<Process>& workload, ResourceManager& rm) override;
private:
    int quantum;
};

class MLFQ : public Scheduler {
public:
    MLFQ() : Scheduler("MLFQ (3 Queues)") {}
    void simulate(std::vector<Process>& workload, ResourceManager& rm) override;
};

#endif // SCHEDULERS_H
