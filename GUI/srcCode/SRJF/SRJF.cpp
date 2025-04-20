#include "SRJF.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include<bits/stdc++.h>

SRJF::SRJF(QObject* parent)
    : QObject(parent) {}

void SRJF::runAlgo(std::queue<Process>& processes, bool live, float& current_time,GanttChart* gantt, std::mutex& mtx) {
    // Priority queue for ready processes (sorted by remaining time)
    auto cmp = [](Process* left, Process* right) {
        return left->remaining_time > right->remaining_time;
    };
    std::priority_queue<Process*, std::vector<Process*>, decltype(cmp)> ready_queue(cmp);
    this->processes=processes;
    std::unordered_set<int> processes_in_queue;
    Process* current_process = nullptr;

    while (true) {
        // Add arriving processes to ready queue
        {
            std::lock_guard<std::mutex> lock(mtx);
            while (!processes.empty() && processes.front().arrival_time <= current_time) {
                Process& p = processes.front();
                ready_queue.push(&p);
                processes_in_queue.insert(p.pid);
                processes.pop();
            }
        }
        /*
        // Process completion check
        if (current_process && current_process->remaining_time == 0) {
            current_process->finish_time = current_time;
            processes_in_queue.erase(current_process->pid);
            current_process = nullptr;
        }
        */
        if (current_process && current_process->remaining_time == 0) {
            std::lock_guard<std::mutex> lock(completed_mutex);
            current_process->finish_time = current_time;
            completed_processes.push_back(*current_process);
            processes_in_queue.erase(current_process->pid);
            current_process = nullptr;
        }

        // Get next process if none running
        if (!current_process && !ready_queue.empty()) {
            current_process = ready_queue.top();
            ready_queue.pop();
            processes_in_queue.erase(current_process->pid);
            if (current_process->start_time == -1) {
                current_process->start_time = current_time;
            }
        }

        // Execute current process for 1 time unit
        if (current_process) {
            current_process->remaining_time--;

            // Update Gantt chart
            {
                std::lock_guard<std::mutex> lock(mtx);
                operate.push(current_process->name);
                time_slots.push({current_time, current_time + 1});

                if (gantt) {
                    std::queue<char> operateCopy = operate;
                    std::queue<std::vector<float>> timeSlotsCopy = time_slots;
                    gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
                    QApplication::processEvents();
                }
            }

            // Check for preemption
            if (!ready_queue.empty() && ready_queue.top()->remaining_time < current_process->remaining_time) {
                ready_queue.push(current_process);
                processes_in_queue.insert(current_process->pid);
                current_process = ready_queue.top();
                ready_queue.pop();
                processes_in_queue.erase(current_process->pid);
                if (current_process->start_time == -1) {
                    current_process->start_time = current_time;
                }
            }
        }

        // Check termination conditions
        bool all_done = true;
        {
            std::lock_guard<std::mutex> lock(mtx);
            all_done = processes.empty() && ready_queue.empty() && (current_process == nullptr);
        }

        if (all_done) {
            break;
        }

        current_time++;
        if (live) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    // Store final results
    std::lock_guard<std::mutex> lock(mtx);

    this->operate = operate;
    this->time_slots = time_slots;
}
/*
QString SRJF::calculateAverages(const std::vector<Process>& processes) {
    double total_waiting = 0;
    double total_turnaround = 0;
    int count = 0;

    for (const auto& p : processes) {
        if (p.finish_time > 0) { // Only count completed processes
            int turnaround = p.finish_time - p.arrival_time;
            int waiting = turnaround - p.burst_time;
            total_turnaround += turnaround;
            total_waiting += waiting;
            count++;
        }
    }

    QString results;
    if (count > 0) {
        results += QString("Average Turnaround Time: %1\n").arg(total_turnaround / count);
        results += QString("Average Waiting Time: %1\n").arg(total_waiting / count);
    } else {
        results = "No processes completed.";
    }
    return results;
}
*/
QString SRJF::calculateAverages() {
    std::lock_guard<std::mutex> lock(completed_mutex);

    if (completed_processes.empty()) {
        return "No processes completed.";
    }

    double total_waiting = 0;
    double total_turnaround = 0;

    for (const auto& p : completed_processes) {
        int turnaround = p.finish_time - p.arrival_time;
        int waiting = turnaround - p.burst_time;
        total_turnaround += turnaround;
        total_waiting += waiting;
    }

    QString results;
    results += QString("Average Turnaround Time: %1\n").arg(total_turnaround / completed_processes.size());
    results += QString("Average Waiting Time: %1\n").arg(total_waiting / completed_processes.size());
    return results;
}
/*
QString SRJF::printResults() {
    printGantt(operate, time_slots, false);
    vector<Process> pout;
    // Convert queue to vector for calculations
    while(!processes.empty()){
        pout.push_back(processes.front());
        processes.pop();


    }
    // Note: You'll need to maintain completed processes separately in your class

    return calculateAverages(pout);
}
*/
QString SRJF::printResults() {
    printGantt(operate, time_slots, false);
    return calculateAverages();
}
