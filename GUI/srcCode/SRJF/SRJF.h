#ifndef SRJF_H
#define SRJF_H

#include <vector>
#include <queue>
#include <atomic>
#include <mutex>

// Structure to represent a Process
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;

    Process(int pid, int arrival, int burst);
};

// Comparator for the priority queue (Shortest Remaining Job First)
extern auto cmp;

// Function to calculate average waiting time and average turnaround time
void calculateAverages(const std::vector<Process>& processes);

// Function to simulate SRJF scheduling
void scheduleSRJF(std::vector<Process>& processes);

// Function to handle dynamic input from the user (runs in a separate thread)
void inputListener();

// Global variables for thread synchronization
extern std::mutex mtx;
extern std::vector<Process> dynamic_processes;
extern std::atomic<bool> stop_flag;

#endif // SRJF_H
