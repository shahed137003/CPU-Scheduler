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
    overall_time=0;
    qDebug() << "processStep called, readyQueue size:" << readyQueue.size() << "processes size:" << processes.size();
    bool first = false;
    while (!readyQueue.empty() || !processes.empty() || !stopInput) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            while (!processes.empty() && processes.front().getArrival() <= overall_time) {
                readyQueue.push(processes.front());
                processes.pop();
            }
        }

        if (readyQueue.empty()) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (!processes.empty()) {
                    operate.push('#');
                    time_slots.push({overall_time,processes.front().getArrival()});
                    overall_time = processes.front().getArrival();
                } else {
                    break;
                }
            }
            continue;
        }

        operating = readyQueue.front();
        readyQueue.pop();

        if (operating.getResponse() < 0) {
            operating.setResponse(overall_time - operating.getArrival());
        }

        float time_slice = (((quantum) < (operating.getRemaining())) ? (quantum) : (operating.getRemaining()));
        if(first){
            if(overall_time != 0){
                operate.push('#');
                time_slots.push({0,overall_time});
            }
            first = false;
        }
        // Debug the current state of queues
        qDebug() << "Before update: operate size:" << operate.size() << "time_slots size:" << time_slots.size();
        operate.push(operating.getName());
        time_slots.push({overall_time, overall_time + time_slice});


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
        }

        if (live) {
            wait_ms(100);
        }
    }
    printResults();
}

QString RoundRobin::printResults() {
    processes = terminatedProcesses;

    // Output results
    printGantt(operate, time_slots, live);

    cout << "\n\n\n";
    cout << "\nTotal Response Time: " << calcTotal_response_time(processes) << "\n";
    cout << "Average Response Time: " << calcAvg_response_time(processes) << "\n\n";
    cout << "Total Turnaround Time: " << calcTotal_turn_time(processes) << "\n";
    cout << "Average Turnaround Time: " << calcAvg_turn_time(processes) << "\n\n";
    cout << "Total Waiting Time: " << calcTotal_wait_time(processes) << "\n";
    cout << "Average Waiting Time: " << calcAvg_wait_time(processes) << "\n";    QString results;
    results += QString("Total Turnaround Time: %1\n").arg(calcTotal_turn_time(terminatedProcesses));
    results += QString("Average Turnaround Time: %1\n\n").arg(calcAvg_turn_time(terminatedProcesses));
    results += QString("Total Waiting Time: %1\n").arg(calcTotal_wait_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n\n").arg(calcAvg_wait_time(terminatedProcesses));
    results += QString("Total Waiting Time: %1\n").arg(calcTotal_response_time(terminatedProcesses));
    results += QString("Average Response Time: %1\n").arg(calcAvg_response_time(terminatedProcesses));

    return results;
}
