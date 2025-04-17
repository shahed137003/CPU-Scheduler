#include "SRJF.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <iomanip>

SRJF::SRJF(std::vector<Process>& initialProcesses, bool live, GanttChart* gantt, QObject* parent)
    : QObject(parent),processes(initialProcesses),ready_queue([](Process* left, Process* right) {
        return left->remaining_time > right->remaining_time;}),current_process(nullptr),current_time(0),live(live),gantt(gantt),
    initial_only(true),stop_flag(false),new_processes_added(false) {
    connect(this, &SRJF::requestProcessStep, this, &SRJF::processStep);
    //connect(&timer, &QTimer::timeout, this, );
}

void SRJF::start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    emit requestProcessStep();
}

void SRJF::processStep() {
    while (true) {
        // Check for new processes
        {
            lock_guard<mutex> lock(mtx);
            while (!new_processes.empty() && new_processes.front().arrival_time <= current_time) {
                Process& p = new_processes.front();
                processes.push_back(p);
                ready_queue.push(&processes.back());
                new_processes.pop();
                if (!p.is_initial) {
                    new_processes_added = true;
                    initial_only = false;
                }
            }
        }

        // Check for arriving processes
        for (auto& p : processes) {
            if (p.arrival_time == current_time && p.remaining_time == p.burst_time) {
                ready_queue.push(&p);
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
            operate.push(char(current_process->name));
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
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }

        // Check termination conditions
        bool should_stop = false;
        {
            lock_guard<mutex> lock(mtx);
            should_stop = stop_flag ||
                          (initial_only && ready_queue.empty() && (!current_process || current_process->remaining_time == 0));
        }

        if (should_stop) {
            if (!stop_flag && initial_only) {
                cout << "\nAll initial processes completed at time " << current_time << endl;
            }
            printResults();
            break;
        }

        current_time++;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

QString SRJF::calculateAverages() {
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

    QString results;
    //results += QString("Total Turnaround Time: %1\n").arg(calcTotal_turn_time(terminatedProcesses));
    results += QString("Average Turnaround Time: %1\n\n").arg((total_turnaround / count));
    //results += QString("Total Waiting Time: %1\n").arg(calcTotal_wait_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n").arg((total_waiting / count));
    return results;

}

QString SRJF::printResults() {
    // Output results
    printGantt(operate, time_slots, live);

    return calculateAverages();
}
