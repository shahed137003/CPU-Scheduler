#ifndef SRJF_SCHEDULER_H
#define SRJF_SCHEDULER_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Struct to represent a process
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time = -1;
    int completion_time;
};

// Comparison function for sorting by arrival time
bool compareArrival(const Process& a, const Process& b);

// Function declaration for SRJF scheduling
void srjfScheduler(vector<Process>& processes);

#endif // SRJF_SCHEDULER_H
