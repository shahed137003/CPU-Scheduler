#include "PriorityPre.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

PriorityPre::PriorityPre(std::vector<Processes>& initialProcesses, bool live, GanttChart* gantt, QObject* parent)
    : QObject(parent), live(live), gantt(gantt) {
    // Initialize processes with remaining time
    while (!initialProcesses.empty()) {

            Processes p = initialProcesses.front(); // get the first element
            p.setRemaining(p.getBurst());
            processes.push_back(p);
            initialProcesses.erase(initialProcesses.begin()); // remove the first element


    }
    connect(this, &PriorityPre::requestProcessStep, this, &PriorityPre::processStep);
}

void PriorityPre::start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    emit requestProcessStep();
}

void PriorityPre::processStep() {
    qDebug() << "PriorityPre::processStep called, processes size:" << processes.size();
    float total_burst = 0;
    float first_arr = processes[0].getArrival(); // assuming the user enters processes by order of arrival
    for (auto& process : processes) {
        process.setRemaining(process.getBurst());
        qDebug()<<"BURST"<<process.getBurst();
        total_burst += process.getBurst();
        if(process.getArrival()<first_arr) first_arr = process.getArrival(); // Just in case the user enters the processes randomly
    }

    for (float i = 0; i < total_burst+first_arr; i++) {
        int selected = -1;
        int x = processes.size();
        for (int j = 0; j < x; j++) {
            if (processes[j].getArrival() <= i && processes[j].getRemaining() > 0) {
                if ((selected == -1 ||
                    (selected !=-1 && processes[j].getPriority() < processes[selected].getPriority()) ||
                    (selected!=1 && processes[j].getPriority() == processes[selected].getPriority() &&
                     processes[j].getArrival() < processes[selected].getArrival())) && processes[j].getRemaining()>0) {
                    selected = j; ///////
                }
            }
        }

        if (selected != -1) {
            Processes& process = processes[selected];
            process.setRemaining(process.getRemaining() - 1);

            if (process.getRemaining() == 0 && process.getLasttime() == -1) {
                float arr = process.getArrival();
                float burst = process.getBurst();
                float turnaround = i+1 - arr;
                float waiting = turnaround - burst;
                process.setLasttime(i+1);
                process.setTurnaround(turnaround);
                process.setWaiting(waiting);
            }
            operate.push(process.getName());
            time_slots.push({i,i+1}); ////////

            // Update Gantt chart
            if (gantt && live) {
                std::queue<char> operateCopy = operate;
                std::queue<std::vector<float>> timeSlotsCopy = time_slots;
                qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
                gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
                QApplication::processEvents();
            }


        }
    }


    printResults();
}

QString PriorityPre::printResults() {
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
