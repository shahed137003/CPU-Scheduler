#include "RoundRobin.h"
#include <algorithm>
#include <chrono>
#include <thread>

float overall_time = 0.0;
std::queue<Processes> readyQueue;
std::mutex queueMutex;
std::atomic<bool> stopInput(false);

void dynamicInput(std::queue<Processes>& processes, std::mutex& queueMutex, std::atomic<bool>& stopInput) {
    while (!stopInput) {
        std::string input;
        std::cout << "\n[Dynamic Add] Enter process (Name arrival Burst) or type 'STOP' to finish: ";

        std::cin >> input;

        if (input == "STOP" || (processes.empty() && readyQueue.empty())) {
            if(input == "STOP") std::cout << "You Have Stopped the Dynamic Input..\n";
            else if (processes.empty() && readyQueue.empty())
                std::cout << "Last process is not added as all processes finished..\n";
            stopInput = true;
            break;
        }

        char name = input[0];
        float arrival, burst;
        std::cin >> arrival >> burst;

        if (std::cin.fail() || burst <= 0 || arrival < 0) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cout << "Invalid input. Please ensure arrival and burst times are valid.\n";
            continue;
        }
        if (arrival < overall_time) {
            arrival = overall_time;
            std::cout << "You chose an arrival time less than the running time now.\n"
                      << "So, we will set arrival time of process " << name << " to " << overall_time << "\n";
        }
        Processes newProc(name, arrival, burst);
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            processes.push(newProc);
            // Assuming sort_queue is defined elsewhere
            // sort_queue(processes);
        }

        std::cout << "[Process " << name << " added dynamically]\n" << overall_time << "\n";
    }
}

void roundRobin(std::queue<Processes>& processes, float quantum, bool live) {
    std::queue<std::vector<float>> time_slots;
    std::queue<char> operate;
    std::queue<Processes> terminatedProcesses;
    Processes operating;
    std::thread inputThread(dynamicInput, std::ref(processes), std::ref(queueMutex), std::ref(stopInput));

    while (!readyQueue.empty() || !processes.empty() || !stopInput) {
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
                    // operate.push('#');
                    // time_slots.push({overall_time,processes.front().getArrival()});
                    // overall_time = processes.front().getArrival();
                    if(processes.front().getArrival() - overall_time <= 1){
                        wait_ms(1000*(processes.front().getArrival() - overall_time));
                        overall_time = processes.front().getArrival();
                        operate.push('#');
                        if(time_slots.empty()){
                            time_slots.push({0,overall_time});
                        }
                        else{
                            time_slots.push({time_slots.front()[1],overall_time});
                        }
                    }
                    else{
                        overall_time++;
                        wait(1);
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
        
        operate.push(operating.getName());
        time_slots.push({overall_time, overall_time + time_slice});

        operating.setLasttime(overall_time+time_slice);
        operating.setRemaining(operating.getRemaining() - time_slice);

        if (operating.getRemaining() > 0) {
            readyQueue.push(operating);
        } else {
            operating.setTurnaround(overall_time - operating.getArrival());
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
                    overall_time+=time_slice;
                    wait_ms(1000*time_slice);
                    time_slice--;
                }
            }
        }
        else overall_time += time_slice;
    }

    stopInput = true;
    inputThread.detach();

    processes = terminatedProcesses;

    // Assuming these functions are defined elsewhere
    printGantt(operate, time_slots, live);
    std::cout << "\n\n\n";
    std::cout << "\nTotal Response Time: " <<  calcTotal_response_time(processes) << "\n";
    std::cout << "Average Response Time: " <<  calcAvg_response_time(processes)  << "\n\n";
    std::cout << "Total Turnaround Time: " <<  calcTotal_turn_time(processes)  << "\n";
    std::cout << "Average Turnaround Time: " <<  calcAvg_turn_time(processes)  << "\n\n";
    std::cout << "Total Waiting Time: " << calcTotal_wait_time(processes)  << "\n";
    std::cout << "Average Waiting Time: " <<  calcAvg_wait_time(processes)  << "\n";
}

int main() {
    cout << "Welcome! Enter number of processes to be scheduled: ";
    int n;
    cin >> n;

    if (n < 1) {
        cout << "Please Enter an integer number greater than 0...\n";
        return -1;
    }
    

    queue<Processes> processes;
    for (int i = 0; i < n; i++) {
        char name;
        float arrival, burst;
        cout << "Enter process name, arrival time, and burst time (e.g., A 0 4): ";
        cin >> name >> arrival >> burst;
        Processes temp(name, arrival, burst);
        processes.push(temp);
    }
    sort_queue(processes);


    float quantum;
    cout << "Enter Time Quantum: ";
    cin >> quantum;

    char ans;
    bool live = false;
    cout << "Do you want to display a live Gantt chart? (Y/N): ";
    cin >> ans;

    if (ans == 'Y' || ans == 'y') {
        live = true;
    }
    else if (ans == 'N' || ans == 'n') {
        live = false;
    }
    else {
        cout << "Invalid input, defaulting to No.\n";
    }
    cout << "\nYou can add new processes while the scheduler is running (e.g., Z 5 2)\n\n";

    roundRobin(processes, quantum, live);

    return 0;
}
