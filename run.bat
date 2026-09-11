@echo off
echo Compiling CPU Scheduler Simulator...
C:\MinGW\bin\g++.exe -std=c++14 main.cpp ResourceManager.cpp Schedulers.cpp SimulationManager.cpp -o SchedulerSimulator.exe

if %errorlevel% neq 0 (
    echo.
    echo Compilation failed! Please check your code.
    pause
    exit /b %errorlevel%
)

echo Compilation successful! Running simulation...
echo ========================================================================================
SchedulerSimulator.exe
echo ========================================================================================
pause
