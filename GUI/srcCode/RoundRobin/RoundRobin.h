
#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include <queue>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <string>
#include <iostream>
#include "GanttChart.h" // Include GanttChart definition
#include"..\Processes\Processes.h"

extern float overall_time;
extern std::queue<Processes> readyQueue;
extern std::mutex queueMutex;
extern std::atomic<bool> stopInput;

void dynamicInput(std::queue<Processes>& processes, std::mutex& queueMutex, std::atomic<bool>& stopInput);
void roundRobin(std::queue<Processes>& processes, float quantum, bool live, GanttChart* ganttChart);

#endif // ROUNDROBIN_H
