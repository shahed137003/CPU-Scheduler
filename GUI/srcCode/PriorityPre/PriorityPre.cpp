#include "PriorityPre.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

PriorityPre::PriorityPre(QObject* parent)
    : QObject(parent) {};

void PriorityPre::runAlgo(std::vector<Processes>& initialProcesses, bool live, float& overall_time, GanttChart* gantt, std::mutex& vectorMutex) {
    // Initialize processes from initialProcesses
    {
        std::lock_guard<std::mutex> lock(vectorMutex);
        while (!initialProcesses.empty()) {
            Processes p = initialProcesses.front();
            p.setRemaining(p.getBurst());
            processes.push_back(p);
            initialProcesses.erase(initialProcesses.begin());
        }
    }

    float total_burst = 0;
    float first_arr = processes.empty() ? 0 : processes[0].getArrival();
    for (auto& process : processes) {
        process.setRemaining(process.getBurst());
        qDebug() << "BURST" << process.getBurst();
        total_burst += process.getBurst();
        if (process.getArrival() < first_arr) first_arr = process.getArrival();
    }

    qDebug() << "PriorityPre called, processes size:" << processes.size();
    if (overall_time < first_arr) {
        if (live) wait_ms(1000 * (first_arr - overall_time));
        operate.push('#');
        time_slots.push({overall_time, first_arr});
        overall_time = first_arr;
    }

    float timeElapsed = 0;
    while (timeElapsed < total_burst) {
        // Check and append any new processes
        {
            std::lock_guard<std::mutex> lock(vectorMutex);
            for (size_t i = 0; i < initialProcesses.size(); ++i) {
                bool alreadyAdded = false;
                for (const auto& p : processes) {
                    if (p.getName() == initialProcesses[i].getName()) {
                        alreadyAdded = true;
                        break;
                    }
                }
                if (!alreadyAdded && initialProcesses[i].getArrival() <= overall_time) {
                    Processes p = initialProcesses[i];
                    p.setRemaining(p.getBurst());
                    total_burst += p.getBurst();
                    processes.push_back(p);
                }
            }
        }

        int selected = -1;
        bool isIdle = true;
        for (size_t j = 0; j < processes.size(); j++) {
            if (processes[j].getArrival() <= overall_time && processes[j].getRemaining() > 0) {
                isIdle = false;
                if ((selected == -1 ||
                     (selected != -1 && processes[j].getPriority() < processes[selected].getPriority()) ||
                     (selected != -1 && processes[j].getPriority() == processes[selected].getPriority() &&
                      processes[j].getArrival() < processes[selected].getArrival())) &&
                    processes[j].getRemaining() > 0) {
                    selected = j;
                }
            }
        }

        if (selected != -1) {
            Processes& process = processes[selected];
            process.setRemaining(process.getRemaining() - 1);

            if(!operate.empty() && last == process.getName())
            {
                vector<float>lastTimeSlot;
                std::queue<vector<float>> tempQueue;
                while (!time_slots.empty()) {
                    lastTimeSlot = time_slots.front();
                    time_slots.pop();
                    if (!time_slots.empty()) {
                        tempQueue.push(lastTimeSlot); // Keep all except the last
                    }
                }
                // Push back
                while (!tempQueue.empty()) {
                    time_slots.push(tempQueue.front());
                    tempQueue.pop();
                }
                // Push updated time slot with same start time, new end time
                time_slots.push({lastTimeSlot[0], overall_time + 1});

            }
            else{
                operate.push(process.getName());
                last = process.getName();
                time_slots.push({overall_time, overall_time + 1});
            }

            if (process.getRemaining() == 0 && process.getLasttime() == -1) {
                float arr = process.getArrival();
                float burst = process.getBurst();
                float turnaround = overall_time + 1 - arr;
                float waiting = turnaround - burst;
                process.setLasttime(overall_time + 1);
                process.setTurnaround(turnaround);
                process.setWaiting(waiting);

                {
                    std::lock_guard<std::mutex> lock(vectorMutex);
                    terminatedProcesses.push(process);
                }
            }
        } else if (isIdle) {
            // Add idle period
            operate.push('#');
            time_slots.push({overall_time, overall_time + 1});
        }

        if (gantt) {
            std::queue<char> operateCopy = operate;
            std::queue<std::vector<float>> timeSlotsCopy = time_slots;
            qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
            gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
            QApplication::processEvents();
        }

        if (live) wait_ms(1000);
        overall_time += 1;
        if (selected != -1) timeElapsed += 1;
    }

    printResults();
    this->operate = operate;
    this->terminatedProcesses = terminatedProcesses;
    this->time_slots = time_slots;
}

QString PriorityPre::printResults() {
    queue<Processes>processes = this->terminatedProcesses;
    queue<char>operate = this->operate;
    queue<vector<float>>time_slots = this->time_slots;
    // Output results
    printGantt(operate, time_slots, false);

    std::cout << "\n\n\n";
    std::cout << "Total Turnaround Time: " << calcTotal_turn_time(terminatedProcesses) << "\n";
    std::cout << "Average Turnaround Time: " << calcAvg_turn_time(terminatedProcesses) << "\n\n";
    std::cout << "Total Waiting Time: " << calcTotal_wait_time(terminatedProcesses) << "\n";
    std::cout << "Average Waiting Time: " << calcAvg_wait_time(terminatedProcesses) << "\n";

    QString results;
    results += QString("Average Turnaround Time: %1\n\n").arg(calcAvg_turn_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n").arg(calcAvg_wait_time(terminatedProcesses));

    return results;
}
