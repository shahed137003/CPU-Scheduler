#include "RoundRobin.h"
#include <QApplication>
#include <iostream>
#include <QDebug>
using namespace std;

RoundRobin::RoundRobin(QObject* parent)
    : QObject(parent){}

void RoundRobin::runAlgo(std::queue<Processes>& processes,
                         std::queue<std::pair<char,float>>&remaining, float quantum, bool live, float& overall_time,GanttChart* gantt,
                         std::mutex& queueMutex) {
    qDebug() << "processStep called, readyQueue size:" << readyQueue.size() << "processes size:" << processes.size();
    while (!readyQueue.empty() || !processes.empty()) {

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            while (!processes.empty() && processes.front().getArrival() <= overall_time) {
                if( processes.front().getArrival() < overall_time){
                    int j = readyQueue.size();
                    Processes temp;
                    for(int i = 0; i < j-1; i++){
                        temp = readyQueue.front();
                        readyQueue.pop();
                        readyQueue.push(temp);
                    }
                    temp = readyQueue.front();
                    readyQueue.pop();
                    readyQueue.push(processes.front());
                    processes.pop();
                    readyQueue.push(temp);
                }else{
                    readyQueue.push(processes.front());
                    processes.pop();
                }
            }
        }

        if (readyQueue.empty()) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (!processes.empty()) {
                    if(processes.front().getArrival() - overall_time <= 1){
                        wait_ms(1000*(processes.front().getArrival() - overall_time));
                        overall_time = processes.front().getArrival();
                        operate.push('#');
                        if(time_slots.empty()){
                            time_slots.push({0,overall_time});
                        }
                        else{
                            vector<float> temp;
                            int n = time_slots.size();
                            for(int i = 0; i < n;i++){
                                temp = time_slots.front();
                                time_slots.pop();
                                time_slots.push(temp);
                            }
                            time_slots.push({temp[1],overall_time});
                        }
                    }
                    else{
                        overall_time++;
                        if(live)wait(1);
                    }
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
        // Debug the current state of queues
        qDebug() << "Before update: operate size:" << operate.size() << "time_slots size:" << time_slots.size();
        if(!operate.empty() && last == operating.getName())
        {
            vector<float>lastTimeSlot;
            std::queue<vector<float>> tempQueue;
            while (!time_slots.empty()) {
                lastTimeSlot = time_slots.front();
                time_slots.pop();
                if (!time_slots.empty()) {
                    tempQueue.push(lastTimeSlot); // Keep all except the last
                }
            }
            // Push back
            while (!tempQueue.empty()) {
                time_slots.push(tempQueue.front());
                tempQueue.pop();
            }
            // Push updated time slot with same start time, new end time
            time_slots.push({lastTimeSlot[0], overall_time + time_slice});

        }
        else{
            operate.push(operating.getName());
            last = operating.getName();
            time_slots.push({overall_time, overall_time + time_slice});
        }


        // Create copies for Gantt chart update
        std::queue<char> operateCopy = operate;
        std::queue<std::vector<float>> timeSlotsCopy = time_slots;

        // Update Gantt chart
        if (gantt) {
            std::queue<char> operateCopy = operate;
            std::queue<std::vector<float>> timeSlotsCopy = time_slots;
            qDebug() << "Updating GanttChart with copy, operateCopy size:" << operateCopy.size();
            gantt->updateGanttChart(operateCopy, timeSlotsCopy, true);
            QApplication::processEvents();
        }

        std::cout << "RoundRobin: Scheduling process " << operating.getName()
                  << " start: " << overall_time << " end: " << (overall_time + time_slice) << std::endl;
        std::cout << "RoundRobin: Updated GanttChart with " << operate.size() << " processes" << std::endl;


        operating.setLasttime(overall_time + time_slice);
        operating.setRemaining(operating.getRemaining() - time_slice);
        remaining.push({operating.getName(),operating.getRemaining()});

        if (operating.getRemaining() > 0) {
            readyQueue.push(operating);
        }
        else {
            operating.setTurnaround(overall_time + time_slice - operating.getArrival());
            operating.setWaiting(operating.getTurnaround() - operating.getBurst());
            terminatedProcesses.push(operating);
        }

        if (live) {
            while(time_slice){
                if(time_slice <= 1){
                    overall_time+=time_slice;
                    wait_ms(1000*time_slice);
                    break;
                }
                else{
                    overall_time++;
                    wait_ms(1000*time_slice);
                    time_slice--;
                }
            }
        }
        else overall_time += time_slice;
    }
    printResults();
    this->operate = operate;
    this->terminatedProcesses = terminatedProcesses;
    this->time_slots = time_slots;
}

QString RoundRobin::printResults() {
    queue<Processes>processes = this->terminatedProcesses;
    queue<char>operate = this->operate;
    queue<vector<float>>time_slots = this->time_slots;
    // Output results
    printGantt(operate, time_slots, false);

    cout << "\n\n\n";
    cout << "\nTotal Response Time: " << calcTotal_response_time(processes) << "\n";
    cout << "Average Response Time: " << calcAvg_response_time(processes) << "\n\n";
    cout << "Total Turnaround Time: " << calcTotal_turn_time(processes) << "\n";
    cout << "Average Turnaround Time: " << calcAvg_turn_time(processes) << "\n\n";
    cout << "Total Waiting Time: " << calcTotal_wait_time(processes) << "\n";
    cout << "Average Waiting Time: " << calcAvg_wait_time(processes) << "\n";
    QString results;
    results += QString("Total Turnaround Time: %1\n").arg(calcTotal_turn_time(terminatedProcesses));
    results += QString("Average Turnaround Time: %1\n\n").arg(calcAvg_turn_time(terminatedProcesses));
    results += QString("Total Waiting Time: %1\n").arg(calcTotal_wait_time(terminatedProcesses));
    results += QString("Average Waiting Time: %1\n\n").arg(calcAvg_wait_time(terminatedProcesses));
    results += QString("Total Waiting Time: %1\n").arg(calcTotal_response_time(terminatedProcesses));
    results += QString("Average Response Time: %1\n").arg(calcAvg_response_time(terminatedProcesses));

    return results;
}
