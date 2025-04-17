#include "RoundRobin.h"
#include <QApplication>
#include <iostream>
#include <QDebug>
using namespace std;

RoundRobin::RoundRobin(std::queue<Processes>& initialProcesses, float quantum, bool live, GanttChart* gantt, QObject* parent)
    : QObject(parent), processes(initialProcesses), quantum(quantum), live(live), gantt(gantt){
    connect(this, &RoundRobin::requestProcessStep, this, &RoundRobin::processStep);
    //connect(&timer, &QTimer::timeout, this, );
}

void RoundRobin::start() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    emit requestProcessStep();
}

void RoundRobin::processStep() {
    qDebug() << "processStep called, readyQueue size:" << readyQueue.size() << "processes size:" << processes.size();
    while (!readyQueue.empty() || !processes.empty() || !stopInput) {
        // Move processes to readyQueue based on arrival times
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            while (!processes.empty() && processes.front().getArrival() <= overall_time) {
                readyQueue.push(processes.front());
                processes.pop();
            }
        }

        if (readyQueue.empty()) {
            // Advance time to the next process arrival if readyQueue is empty
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (!processes.empty()) {
                    overall_time = processes.front().getArrival();
                    continue;
                } else if (stopInput) {
                    printResults();
                    return;
                }
            }
        }

        operating = readyQueue.front();
        readyQueue.pop();

        if (operating.getResponse() < 0) {
            operating.setResponse(overall_time - operating.getArrival());
        }


        // Calculate time slice
        float time_slice = min(quantum, operating.getRemaining());
        operate.push(operating.getName());
        qDebug()<<operating.getName();
        time_slots.push({overall_time, overall_time + time_slice});
        qDebug()<<overall_time;
        qDebug()<<overall_time + time_slice;

        // Debug the current state of queues
        qDebug() << "Before update: operate size:" << operate.size() << "time_slots size:" << time_slots.size();

        // Create copies for Gantt chart update
        std::queue<char> operateCopy = operate;
        std::queue<std::vector<float>> timeSlotsCopy = time_slots;

        // Update Gantt chart
        if (gantt && live) {
            qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
            gantt->updateGanttChart(operateCopy, timeSlotsCopy, live);
            QApplication::processEvents(); // Force GUI update
        }

        std::cout << "RoundRobin: Scheduling process " << operating.getName()
                  << " start: " << overall_time << " end: " << (overall_time + time_slice) << std::endl;
        std::cout << "RoundRobin: Updated GanttChart with " << operate.size() << " processes" << std::endl;

        overall_time += time_slice;
        operating.setLasttime(overall_time);
        operating.setRemaining(operating.getRemaining() - time_slice);

        if (operating.getRemaining() > 0) {
            readyQueue.push(operating);
        } else {
            operating.setTurnaround(overall_time - operating.getArrival());
            operating.setWaiting(operating.getTurnaround() - operating.getBurst());
            terminatedProcesses.push(operating);
            isOperating = false;
        }

        if (live) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(time_slice * 1000)));
        }
    }
    printResults();
}

void RoundRobin::printResults() {
    processes = terminatedProcesses;

    // Output results
    printGantt(operate, time_slots, live);

    cout << "\n\n\n";
    cout << "\nTotal Response Time: " << calcTotal_response_time(processes) << "\n";
    cout << "Average Response Time: " << calcAvg_response_time(processes) << "\n\n";
    cout << "Total Turnaround Time: " << calcTotal_turn_time(processes) << "\n";
    cout << "Average Turnaround Time: " << calcAvg_turn_time(processes) << "\n\n";
    cout << "Total Waiting Time: " << calcTotal_wait_time(processes) << "\n";
    cout << "Average Waiting Time: " << calcAvg_wait_time(processes) << "\n";
}
