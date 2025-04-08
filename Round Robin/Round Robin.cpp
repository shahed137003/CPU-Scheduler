#include "Round Robin.h"

using namespace std;

void roundRobin(queue<Processes>& processes, float quantum, bool live) {
    float overall_time = 0.0;
    queue<vector<float>> time_slots;  //to store start and end time of existing of a process in ready queue for gantt chart
    queue<char>operate;
    queue<Processes> readyQueue;
    queue<Processes> terminatedProcesses;
    Processes operating;

    while (!readyQueue.empty() || !processes.empty()) {
        // Move processes to readyQueue based on arrival times
        while (!processes.empty() && processes.front().getArrival() <= overall_time) {
            readyQueue.push(processes.front());
            processes.pop();
        }

        if (readyQueue.empty()) {
            // Advance time to the next process arrival if readyQueue is empty
            overall_time = processes.front().getArrival();
            continue;
        }

        operating = readyQueue.front();
        readyQueue.pop();

        if (operating.getResponse() < 0) {
            operating.setResponse(overall_time - operating.getArrival());
        }

        // Calculate time slice
        float time_slice = min(quantum, operating.getRemaining());
        /*if (time_slots.empty() && overall_time != 0) {
            time_slots.push({ 0,overall_time });
            operate.push('#');
        }*/
        operate.push(operating.getName());
        time_slots.push({ overall_time,overall_time + time_slice });
        overall_time += time_slice;
        operating.setLasttime(overall_time);
        operating.setRemaining(operating.getRemaining() - time_slice);


        if (operating.getRemaining() > 0) {
            readyQueue.push(operating);
        }
        else {
            operating.setTurnaround(overall_time - operating.getArrival());
            operating.setWaiting(operating.getTurnaround() - operating.getBurst());
            terminatedProcesses.push(operating);
        }
    }

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

int main() {
    cout << "Welcome! Enter number of processes to be scheduled: ";
    int n;
    cin >> n;

    vector<Processes> processes_vec;
    for (int i = 0; i < n; i++) {
        char name;
        float arrival, burst;
        cout << "Enter process name, arrival time, and burst time (e.g., A 0 4): ";
        cin >> name >> arrival >> burst;
        processes_vec.emplace_back(name, arrival, burst);
    }

    sort(processes_vec.begin(), processes_vec.end(), compareByArrival);

    queue<Processes> processes;
    for (const auto& proc : processes_vec) {
        processes.push(proc);
    }

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
    cout << "\n";
    roundRobin(processes, quantum, live);

    return 0;
}