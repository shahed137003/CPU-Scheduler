#ifndef SJF_H
#define SJF_H
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
struct Process {
    string name;
    float arrival_time;
    float burst_time;
    float remaining_time;
    float completion_time;
    float turn_around_time;
    float waiting_time;
};

void calc_avg_turn_around_time(vector<Process> processes);
void calc_avg_waiting_time(vector<Process> processes);
void SJF(vector<Process> processes);

#endif
