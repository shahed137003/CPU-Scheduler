#include "PriorityNon.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>



PriorityNon::PriorityNon(QObject* parent)
    : QObject(parent) {};


void PriorityNon::runAlgo(std::vector<Processes>& initialProcesses, bool live, float& overall_time, GanttChart* gantt, std::mutex& vectorMutex){


    qDebug() << "PriorityNon::processStep called, processes size:" << processes.size();
    // Sort by priority
    {
        std::lock_guard<std::mutex> lock(vectorMutex);
        std::sort(processes.begin(), processes.end(),compareByPriority);

    }

    {
        std::lock_guard<std::mutex> lock(vectorMutex);
        // Push back to readyQueue
        for (const auto& p : processes) {
            readyQueue.push(p);
        }

    }

    int vector_size = processes.size();
    while (!readyQueue.empty() || processes.size() > vector_size) {

        {
            std::lock_guard<std::mutex> lock(vectorMutex);
            while( processes.size() > vector_size){
                readyQueue.push(processes[vector_size]);
                vector_size++;
            }
        }


        Processes operating = readyQueue.front();
        readyQueue.pop();

        // Adjust overall_time if process hasn't arrived yet
        if (operating.getArrival() > overall_time)
        {
         if(live)wait_ms(1000*(operating.getArrival() - overall_time));
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
        if (gantt) {
            std::queue<char> operateCopy = operate;
            std::queue<std::vector<float>> timeSlotsCopy = time_slots;
            qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
            gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
            QApplication::processEvents();
        }

        if (live) {
            int i = operating.getBurst();
            while(i--){
                wait(1);
            }
        }
        std::cout << "PriorityNon: Scheduling process " << operating.getName()
                  << " start: " << start_time << " end: " << end_time << std::endl;
        std::cout << "PriorityNon: Updated GanttChart with " << operate.size() << " processes" << std::endl;

        terminatedProcesses.push(operating);

    }
    printResults();
    this->operate = operate;
    this->terminatedProcesses = terminatedProcesses;
    this->time_slots = time_slots;

}

QString PriorityNon::printResults() {
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
