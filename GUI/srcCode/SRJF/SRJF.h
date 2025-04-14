// scheduler.h

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iomanip>
#include <atomic>
#include <string>
# include <QString>
// Process structure definition
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    bool is_initial;

    Process(int id, int arrival, int burst, bool initial);
};

// Global variables
extern std::mutex mtx;
extern std::queue<Process> new_processes;
extern std::atomic<bool> stop_flag;
extern std::atomic<bool> new_processes_added;

// Function prototypes
void calculateAverages(const std::vector<Process>& processes);
void scheduleSRJF(std::vector<Process>& processes);
void inputListener();

#endif // SCHEDULER_H
