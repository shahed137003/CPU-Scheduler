#pragma once

#include"..\Processes\Processes.h"

void roundRobin(queue<Processes>& processes, float quantum, bool live, GanttChart* ganttChart, bool gui);
