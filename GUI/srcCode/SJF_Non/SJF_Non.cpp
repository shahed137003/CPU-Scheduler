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
        float minBurst = std::numeric_limits<float>::max();

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].getArrival() <= overall_time && processes[i].getBurst() < minBurst) {
                minBurst = processes[i].getBurst();
                index = i;
            }
        }

        if (index != -1) {
            // Process found, perform scheduling
            Processes p = processes[index];
            processes.erase(processes.begin() + index);

            start_time = overall_time;
            finish_time = start_time + p.getBurst();

            p.setTurnaround(finish_time - p.getArrival());
            p.setWaiting(p.getTurnaround() - p.getBurst());
            p.setLasttime(finish_time);

            operate.push(p.getName());
            time_slots.push({ start_time, finish_time });

            if (gantt) {
                queue<char> operateCopy = operate;
                queue<vector<float>> timeSlotsCopy = time_slots;
                gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
                QApplication::processEvents(); // Force GUI update
            }

            if (live) {
                int burstInt = static_cast<int>(p.getBurst());
                for (int sec = 0; sec < burstInt; ++sec) {
                    wait_ms(1000);        // Wait 1 second
                    overall_time += 1.0;  // Advance time by 1
                    QApplication::processEvents();
                }

                // If burst time is not an integer, process remaining fraction
                float remaining = p.getBurst() - burstInt;
                if (remaining > 0.0f) {
                    wait_ms(remaining * 1000);
                    overall_time += remaining;
                }
            } else {
                overall_time = finish_time;
            }

            std::cout << "SJF_Non: Scheduling process " << p.getName()
                      << " start: " << start_time << " end: " << finish_time << std::endl;

            terminatedProcesses.push(p);
        } else {
            // No process is ready – CPU is idle
            float nextArrival = std::numeric_limits<float>::max();
            for (const auto& p : processes) {
                if (p.getArrival() > overall_time && p.getArrival() < nextArrival) {
                    nextArrival = p.getArrival();
                }
            }

            float idle_time = nextArrival - overall_time;
            if (idle_time > 0) {
                start_time = overall_time;
                if (live) {
                    int idleInt = static_cast<int>(idle_time);
                    for (int sec = 0; sec < idleInt; ++sec) {
                        wait_ms(1000);
                        overall_time += 1.0;
                        QApplication::processEvents();
                    }

                    float remaining = idle_time - idleInt;
                    if (remaining > 0.0f) {
                        wait_ms(remaining * 1000);
                        overall_time += remaining;
                    }
                } else {
                    overall_time = nextArrival;
                }
                finish_time = overall_time;

                operate.push('#'); // Idle marker
                time_slots.push({ start_time, finish_time });
            } else {
                overall_time += 1.0f;
                if (live) {
                    wait_ms(1000);
                    QApplication::processEvents();
                }
            }
        }
    }

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
