#include "SJF_Non.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

SJF_Non::SJF_Non(std::vector<Processes>& initialProcesses, bool live, GanttChart* gantt, QObject* parent)
    : QObject(parent), processes(initialProcesses), live(live), gantt(gantt) {
    connect(this, &SJF_Non::requestProcessStep, this, &SJF_Non::processStep);
}

void SJF_Non::start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    emit requestProcessStep();
}

void SJF_Non::processStep() {
    qDebug() << "SJF_Non::processStep called, processes size:" << processes.size();

    float current_time = 0;
    std::vector<Processes> remaining = processes;
    std::queue<char> local_operate;
    std::queue<std::vector<float>> local_time_slots;
    std::queue<Processes> terminated;

    while (!remaining.empty()) {
        std::sort(remaining.begin(), remaining.end(), compareByBurst);

        bool found = false;
        for (int i = 0; i < remaining.size(); ++i) {
            if (remaining[i].getArrival() <= current_time) {
                Processes p = remaining[i];
                remaining.erase(remaining.begin() + i);

                local_operate.push(p.getName());
                local_time_slots.push({ current_time, current_time + p.getBurst() });

                float start_time = current_time;
                float finish_time = current_time + p.getBurst();
                p.setTurnaround(finish_time - p.getArrival());
                p.setWaiting(p.getTurnaround() - p.getBurst());
                p.setResponse(start_time - p.getArrival());

                terminated.push(p);
                current_time = finish_time;
                found = true;

                // Update Gantt chart live if enabled
                if (live && gantt) {
                    gantt->updateGanttChart(local_operate, local_time_slots, true);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // simulate delay
                }

                break;
            }
        }

        if (!found) {
            current_time++; // Idle CPU
        }
    }

    // Store for final report and visualization
    operate = local_operate;
    time_slots = local_time_slots;
    terminatedProcesses = terminated;

    // Final update to Gantt chart (non-live or complete chart)
    if (gantt) {
        gantt->updateGanttChart(operate, time_slots, false);
    }

    printResults();
}

QString SJF_Non::printResults() {
    std::cout << "\n\n";
    std::cout << "Total Turnaround Time: " << calcTotal_turn_time(terminatedProcesses) << "\n";
    std::cout << "Average Turnaround Time: " << calcAvg_turn_time(terminatedProcesses) << "\n\n";
    std::cout << "Total Waiting Time: " << calcTotal_wait_time(terminatedProcesses) << "\n";
    std::cout << "Average Waiting Time: " << calcAvg_wait_time(terminatedProcesses) << "\n\n";
    std::cout << "Total Response Time: " << calcTotal_response_time(terminatedProcesses) << "\n";
    std::cout << "Average Response Time: " << calcAvg_response_time(terminatedProcesses) << "\n";

    QString results;
    results += QString("Average Turnaround Time: %1\n").arg(calcAvg_turn_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n").arg(calcAvg_wait_time(terminatedProcesses));
    results += QString("Average Response Time: %1\n").arg(calcAvg_response_time(terminatedProcesses));
    return results;
}

