# OS Process Scheduler & Deadlock Avoidance Simulator

A comprehensive C++ simulation of Operating System process scheduling algorithms combined with a resource management system based on Banker's Algorithm for deadlock avoidance.

## Features

- **CPU Scheduling Algorithms Supported:**
  - First-Come, First-Served (FCFS)
  - Shortest Job First / Shortest Remaining Time First (SJF/SRTF)
  - Priority Scheduling (Preemptive)
  - Round Robin (RR)
  - Multilevel Feedback Queue (MLFQ)
- **Deadlock Avoidance:**
  - Resource manager implementing Banker's Algorithm to ensure the system never enters an unsafe state during process allocation.
- **Metrics & Benchmarking:**
  - Calculates and reports Average Waiting Time, Average Turnaround Time, Average Response Time, and CPU Utilization for comparative analysis.

## Getting Started

### Prerequisites
- A modern C++ compiler (e.g., GCC/MinGW, Clang, or MSVC) supporting C++14 or higher.

### Compiling and Running on Windows
A batch script is provided for quick compilation and execution if you are using MinGW.

1. Ensure `g++` is added to your system's PATH (or installed at `C:\MinGW\bin\g++.exe`).
2. Run the provided batch file from the root directory:
   ```cmd
   .\run.bat
   ```
3. The script will compile the `.cpp` files into `SchedulerSimulator.exe` and execute the simulation automatically, outputting a benchmark report in the terminal.

### Manual Compilation
If you are on Linux, macOS, or not using the batch script, compile the source files manually:

```bash
g++ -std=c++14 main.cpp ResourceManager.cpp Schedulers.cpp SimulationManager.cpp -o SchedulerSimulator
./SchedulerSimulator
```

## Architecture

- `Process.h`: Defines the core `Process` structure holding metrics and resource boundaries.
- `ResourceManager.h`/`.cpp`: Contains the deadlock-avoidance implementation (Banker's Algorithm) checking state safety and granting resources.
- `Schedulers.h`/`.cpp`: Base interface and implementations for the different CPU scheduling strategies.
- `SimulationManager.h`/`.cpp`: Handles the lifecycle of running multiple scheduling strategies against the same workload and reporting the comparative metrics.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
