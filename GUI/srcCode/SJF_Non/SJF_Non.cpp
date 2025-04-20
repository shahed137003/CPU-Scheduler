#include "SJF_Non.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

SJF_Non::SJF_Non(QObject* parent)
    : QObject(parent) {}


void SJF_Non::runAlgo(std::vector<Processes>& processes, bool live, float& overall_time, GanttChart* gantt,
                      std::mutex& vectorMutex) {
    qDebug() << "SJF_Non::runAlgo called, processes size:" << processes.size();

    float start_time = 0;
    float finish_time = 0;

    while (!processes.empty()) {
        std::lock_guard<std::mutex> lock(vectorMutex);

        // Find the shortest burst time among the processes that have arrived
        int index = -1;
        float minBurst = 10000;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].getArrival() <= overall_time && processes[i].getBurst() < minBurst) {
                minBurst = processes[i].getBurst();
                index = i;
            }
        }

        if (index != -1) {
            // Process found, perform scheduling
            // std::lock_guard<std::mutex> lock(vectorMutex);
            Processes p = processes[index];
            processes.erase(processes.begin() + index);

            start_time = overall_time;
            //wait_ms(1000*(p.getBurst() - overall_time));
            overall_time += p.getBurst();
            finish_time = overall_time;

            p.setTurnaround(finish_time - p.getArrival());
            p.setWaiting(p.getTurnaround() - p.getBurst());
            p.setLasttime(finish_time);

            operate.push(p.getName());
            time_slots.push({ start_time, finish_time });
            queue<char> operateCopy = operate;
            queue<vector<float>> timeSlotsCopy = time_slots;
            if (gantt && live) {
                qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
                gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
                QApplication::processEvents(); // Force GUI update
            }

            if (live) {
                int burstTime  = p.getBurst();
                while(burstTime --){
                    wait(1);
                }
            }

            std::cout << "SJF_Non: Scheduling process " << p.getName()
                      << " start: " << start_time << " end: " << finish_time << std::endl;

            terminatedProcesses.push(p);
        } else {
            // No process is ready – CPU is idle
            float nextArrival = std::numeric_limits<float>::max();            for (const auto& p : processes) {
                for (const auto& p : processes) {
                    if (p.getArrival() > overall_time && p.getArrival() < nextArrival) {
                        nextArrival = p.getArrival();
                    }
                }
            }

            float idle_time = nextArrival - overall_time;
            if (idle_time > 0) {
                if (live) wait_ms(1000 * idle_time);
                start_time = overall_time;
                overall_time = nextArrival;
                finish_time = overall_time;

                operate.push('#');
                time_slots.push({ start_time, finish_time });
                //idle_time=0;

            }
            else {
                overall_time++; // fallback, shouldn't really reach here
                if (live) wait(1);
            }

        }
    }

    // Print and return results
    printResults();

    // Save state
    this->operate = operate;
    this->terminatedProcesses = terminatedProcesses;
    this->time_slots = time_slots;
}
QString SJF_Non::printResults() {
    queue<Processes>processes = this->terminatedProcesses;
    queue<char>operate = this->operate;
    queue<vector<float>>time_slots = this->time_slots;
    // Output results
    printGantt(operate, time_slots, false);

    cout << "\n\n\n";
    cout << "Total Turnaround Time: " << calcTotal_turn_time(processes) << "\n";
    cout << "Average Turnaround Time: " << calcAvg_turn_time(processes) << "\n\n";
    cout << "Total Waiting Time: " << calcTotal_wait_time(processes) << "\n";
    cout << "Average Waiting Time: " << calcAvg_wait_time(processes) << "\n";
    QString results;
    results += QString("Total Turnaround Time: %1\n").arg(calcTotal_turn_time(terminatedProcesses));
    results += QString("Average Turnaround Time: %1\n\n").arg(calcAvg_turn_time(terminatedProcesses));
    results += QString("Total Waiting Time: %1\n").arg(calcTotal_wait_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n\n").arg(calcAvg_wait_time(terminatedProcesses));

    return results;
}
