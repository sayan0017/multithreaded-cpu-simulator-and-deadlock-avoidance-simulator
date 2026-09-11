cmake_minimum_required(VERSION 3.10)
project(SchedulerSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Executable
add_executable(SchedulerSimulator
    main.cpp
    ResourceManager.cpp
    Schedulers.cpp
    SimulationManager.cpp
)

target_link_libraries(SchedulerSimulator Threads::Threads)
