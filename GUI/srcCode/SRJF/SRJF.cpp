#include "SRJF.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <iomanip>

SRJF::SRJF(std::vector<Process>& initialProcesses, bool live, GanttChart* gantt,std::queue<char>& processes_name, std::queue<std::vector<float>>& timeSlots, QObject* parent)
    : QObject(parent),processes(initialProcesses),ready_queue([](Process* left, Process* right) {
        return left->remaining_time > right->remaining_time;}),current_process(nullptr),current_time(0),live(live),gantt(gantt),operate(processes_name),time_slots(timeSlots),
    initial_only(true) {

    connect(&timer, &QTimer::timeout, this, &SRJF::processStep);
    timer.start(3000);
    qDebug() << "Timer connected:" << timer.isActive();
}

void SRJF::start() {
    qDebug() << "START SRJF" << processes.size();
    timer.start(3000); // Run every 100ms to allow GUI updates
}

void SRJF::processStep() {
    // Check for arriving processes
    for (auto& p : processes) {
        if (p.arrival_time == current_time && p.remaining_time == p.burst_time) {
            ready_queue.push(&p);
            qDebug() << "processesStep SRJF" << p.pid;
        }
    }

    // Process completion
    if (current_process && current_process->remaining_time == 0) {
        current_process->finish_time = current_time;
        current_process = nullptr;
    }

    // Get next process if none running
    if (!current_process && !ready_queue.empty()) {
        current_process = ready_queue.top();
        ready_queue.pop();
        if (current_process->start_time == -1) {
            current_process->start_time = current_time;
        }
    }

    // Execute current process
    if (current_process) {
        current_process->remaining_time--;

        // Update Gantt chart
        operate.push(char(current_process->pid));
        time_slots.push({static_cast<float>(current_time), static_cast<float>(current_time + 1)});

        if (gantt && live) {
            std::queue<char> operateCopy = operate;
            std::queue<std::vector<float>> timeSlotsCopy = time_slots;
            gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
            QApplication::processEvents(); // Force GUI update
        }

        std::cout << "SRJF: Scheduling process P" << current_process->pid
                  << " at time " << current_time << std::endl;

        // Check for preemption
        if (!ready_queue.empty() && ready_queue.top()->remaining_time < current_process->remaining_time) {
            ready_queue.push(current_process);
            current_process = ready_queue.top();
            ready_queue.pop();
        }

        if (live) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // Check termination conditions
    bool should_stop = initial_only && ready_queue.empty() && (!current_process || current_process->remaining_time == 0);
    if (should_stop) {
        timer.stop();
        printResults();
        return;
    }

    current_time++;
}

void SRJF::calculateAverages() {
    double total_waiting = 0;
    double total_turnaround = 0;
    int count = 0;

    for (const auto& p : processes) {
        if (p.finish_time != -1) {
            int turnaround = p.finish_time - p.arrival_time;
            int waiting = turnaround - p.burst_time;
            total_waiting += waiting;
            total_turnaround += turnaround;
            count++;
        }
    }

    if (count > 0) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nAverage Waiting Time: " << (total_waiting / count) << std::endl;
        std::cout << "Average Turnaround Time: " << (total_turnaround / count) << std::endl;
    } else {
        std::cout << "No processes completed." << std::endl;
    }
}

void SRJF::printResults() {
    // Output results
    printGantt(operate, time_slots, live);
    calculateAverages();
}
