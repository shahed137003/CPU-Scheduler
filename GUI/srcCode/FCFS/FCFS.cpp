#include "FCFS.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

FCFS::FCFS(std::vector<Processes>& initialProcesses, bool live, GanttChart* gantt, QObject* parent)
    : QObject(parent), processes(initialProcesses), live(live), gantt(gantt) {
    connect(this, &FCFS::requestProcessStep, this, &FCFS::processStep);
}

void FCFS::start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    emit requestProcessStep();
}

void FCFS::processStep() {
    qDebug() << "FCFS::processStep called, processes size:" << processes.size();

    // Sort by arrival time
    std::sort(processes.begin(), processes.end(), compareByArrival);

    // Push back to readyQueue
    for (const auto& p : processes) {
        readyQueue.push(p);
    }

    while (!readyQueue.empty()) {
        Processes operating = readyQueue.front();
        readyQueue.pop();

        // Adjust overall_time if process hasn't arrived yet
        if (operating.getArrival() > overall_time) {
            overall_time = operating.getArrival();
        }

        // Record start time
        float start_time = overall_time;
        overall_time += operating.getBurst();
        float end_time = overall_time;

        operating.setLasttime(end_time);
        operating.setTurnaround(end_time - operating.getArrival());
        operating.setWaiting(operating.getTurnaround() - operating.getBurst());

        operate.push(operating.getName());
        time_slots.push({start_time, end_time});

        // Update Gantt chart
        if (gantt && live) {
            std::queue<char> operateCopy = operate;
            std::queue<std::vector<float>> timeSlotsCopy = time_slots;
            qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
            gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
            QApplication::processEvents();
        }

        std::cout << "FCFS: Scheduling process " << operating.getName()
                  << " start: " << start_time << " end: " << end_time << std::endl;
        std::cout << "FCFS: Updated GanttChart with " << operate.size() << " processes" << std::endl;

        terminatedProcesses.push(operating);

        if (live) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(operating.getBurst() * 1000)));
        }
    }
    printResults();
}

QString FCFS::printResults() {
    //processes = terminatedProcesses;

    // Output results
   // printGantt(operate, time_slots, live);

    cout << "\n\n\n";
    cout << "\nTotal Response Time: " << calcTotal_response_time(terminatedProcesses) << "\n";
    cout << "Average Response Time: " << calcAvg_response_time(terminatedProcesses) << "\n\n";
    cout << "Total Turnaround Time: " << calcTotal_turn_time(terminatedProcesses) << "\n";
    cout << "Average Turnaround Time: " << calcAvg_turn_time(terminatedProcesses) << "\n\n";
    cout << "Total Waiting Time: " << calcTotal_wait_time(terminatedProcesses) << "\n";
    cout << "Average Waiting Time: " << calcAvg_wait_time(terminatedProcesses) << "\n";
    QString results;
    //results += QString("Total Turnaround Time: %1\n").arg(calcTotal_turn_time(terminatedProcesses));
    results += QString("Average Turnaround Time: %1\n\n").arg(calcAvg_turn_time(terminatedProcesses));
    //results += QString("Total Waiting Time: %1\n").arg(calcTotal_wait_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n").arg(calcAvg_wait_time(terminatedProcesses));

    return results;
}

