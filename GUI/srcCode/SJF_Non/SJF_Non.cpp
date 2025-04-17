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
    queue<char> ordered_processes_with_names;
    queue<Processes> ordered_processes;
    float current_time = 0;
    queue<vector<float>>time_slots;


    while (!processes.empty())
    {
        // we must sort processes based on burst time first, and arrival time second if burst times are equal
        sort(processes.begin(), processes.end(),compareByBurst);

        // we need to find the process that has arrived and has the shortest burst time
        Processes selected_process;
        bool process_found = false;
        int j = processes.size();
        for (int i = 0; i < j; i++) {
            if (processes[i].getArrival() <= current_time) {
                selected_process = processes[i];
                processes.erase(processes.begin() + i);
                process_found = true;
                break;
            }
        }

        if (process_found) {
            ordered_processes_with_names.push(selected_process.getName());
            ordered_processes.push(selected_process);
            time_slots.push({ current_time,current_time + selected_process.getBurst() });
            current_time += selected_process.getBurst();

        }
        else
            current_time++; //If there is no process ready, increment the current time
    }
    printResults();
}

QString SJF_Non::printResults() {
    //processes = terminatedProcesses;
    printGantt(operate, time_slots, live);

    std::cout << "\n\n\n";
    std::cout << "Total Turnaround Time: " << calcTotal_turn_time(terminatedProcesses) << "\n";
    std::cout << "Average Turnaround Time: " << calcAvg_turn_time(terminatedProcesses) << "\n\n";
    std::cout << "Total Waiting Time: " << calcTotal_wait_time(terminatedProcesses) << "\n";
    std::cout << "Average Waiting Time: " << calcAvg_wait_time(terminatedProcesses) << "\n";
    QString results;
    //results += QString("Total Turnaround Time: %1\n").arg(calcTotal_turn_time(terminatedProcesses));
    results += QString("Average Turnaround Time: %1\n\n").arg(calcAvg_turn_time(terminatedProcesses));
    //results += QString("Total Waiting Time: %1\n").arg(calcTotal_wait_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n").arg(calcAvg_wait_time(terminatedProcesses));

    return results;
}
