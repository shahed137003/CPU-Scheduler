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
    float first_arr = processes[0].getArrival(); // assuming user input is ordered
    for (auto& process : processes) {
        process.setRemaining(process.getBurst());
        qDebug() << "BURST" << process.getBurst();
        total_burst += process.getBurst();
        if (process.getArrival() < first_arr) first_arr = process.getArrival();
    }

    qDebug() << "PriorityPre called, processes size:" << processes.size();
     if (overall_time < first_arr)
      {
      if(live) wait_ms(1000*(first_arr - overall_time));
         overall_time = first_arr;
      }
     // if (readyQueue.empty()) {   ns2l sara
     //     {
     //         std::lock_guard<std::mutex> lock(queueMutex);
     //         if (!processes.empty()) {
     //             if(processes.front().getArrival() - overall_time <= 1){
     //                 wait_ms(1000*(processes.front().getArrival() - overall_time));
     //                 overall_time = processes.front().getArrival();
     //                 operate.push('#');
     //                 if(time_slots.empty()){
     //                     time_slots.push({0,overall_time});
     //                 }
     //                 else{
     //                     time_slots.push({time_slots.front()[1],overall_time});
     //                 }
     //             }
     //             else{
     //                 overall_time++;
     //                 if(live)wait(1);
     //             }
     //         }
     //     }
     //     continue;
     // }

      float timeElapsed = 0;
  while (timeElapsed < total_burst) {
        // Check and append any new processes
        {
            std::lock_guard<std::mutex> lock(vectorMutex);
            while (processes.size() < initialProcesses.size()) {
                Processes p = initialProcesses[processes.size()];
                p.setRemaining(p.getBurst());
                total_burst+=p.getBurst(); // edited
                processes.push_back(p);
            }
        }

        int selected = -1;
        int x = processes.size();
        for (int j = 0; j < x; j++) {
            if (processes[j].getArrival() <= overall_time && processes[j].getRemaining() > 0) {
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

            if (process.getRemaining() == 0 && process.getLasttime() == -1) {
                float arr = process.getArrival();
                float burst = process.getBurst();
                float turnaround = overall_time + 1 - arr;
                float waiting = turnaround - burst;
                process.setLasttime(overall_time + 1);
                process.setTurnaround(turnaround);
                process.setWaiting(waiting);

                // Save to terminated list
                {
                    std::lock_guard<std::mutex> lock(vectorMutex);
                    terminatedProcesses.push(process);
                }
            }

            operate.push(process.getName());
            time_slots.push({overall_time, overall_time + 1});

            if (gantt && live) {
                std::queue<char> operateCopy = operate;
                std::queue<std::vector<float>> timeSlotsCopy = time_slots;
                qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
                gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
                QApplication::processEvents();
            }
        }
        if (live) wait(1);

        overall_time += 1;   // 🕒 advance the simulation clock
        timeElapsed += 1;    // tracking how long we've run
    }

    printResults();
    this->operate = operate;
    this->terminatedProcesses = terminatedProcesses;
    this->time_slots = time_slots;
}

QString PriorityPre::printResults() {
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

