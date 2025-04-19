#include "PriorityNon.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

PriorityNon::PriorityNon(std::vector<Processes>& initialProcesses, bool live, GanttChart* gantt, QObject* parent)
    : QObject(parent), processes(initialProcesses), live(live), gantt(gantt) {
    connect(this, &PriorityNon::requestProcessStep, this, &PriorityNon::processStep);
}

void PriorityNon::start() {
    emit requestProcessStep();
}

void PriorityNon::processStep() {
    qDebug() << "PriorityNon::processStep called, processes size:" << processes.size();

    // Sort by priority
    std::sort(processes.begin(), processes.end(),compareByPriority);

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

        std::cout << "PriorityNon: Scheduling process " << operating.getName()
                  << " start: " << start_time << " end: " << end_time << std::endl;
        std::cout << "PriorityNon: Updated GanttChart with " << operate.size() << " processes" << std::endl;

        terminatedProcesses.push(operating);

        if (live) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(operating.getBurst() * 1000)));
        }
    }
    printResults();
}

QString PriorityNon::printResults() {
    //processes = terminatedProcesses;
    printGantt(operate, time_slots, false);

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
