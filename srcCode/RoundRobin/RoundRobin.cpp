#include "RoundRobin.h"


// Shared mutex and atomic flag for dynamic input thread
float overall_time = 0.0;
queue<Processes> readyQueue;
std::mutex queueMutex;
std::atomic<bool> stopInput(false);

// Function to handle dynamic process input
void dynamicInput(queue<Processes>& processes, std::mutex& queueMutex, std::atomic<bool>& stopInput) {
    while (!stopInput) {
        string input;
        cout << "\n[Dynamic Add] Enter process (Name arrival Burst) or type 'STOP' to finish: ";
        
        cin >> input;

        if (input == "STOP" ||(processes.empty() && readyQueue.empty())) { // Check if the user wants to stop entering processes
            if (processes.empty() && readyQueue.empty())
                cout << "Last processes is not added as all processes finished..\n";
            stopInput = true; // Signal the input thread to terminate
            break;
        }

        char name = input[0];
        float arrival, burst;
        cin >> arrival >> burst;

        if (cin.fail() || burst <= 0 || arrival < 0) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid input. Please ensure arrival and burst times are valid.\n";
            continue;
        }
        if (arrival < overall_time) {
            arrival = overall_time;
            cout << "You choose an arrival time less than the running time now.\n"
                <<"So, we will set arrival time of process "<<name<<" to "<<overall_time<<"\n";
        }
        Processes newProc(name, arrival, burst);
        {
            {
                lock_guard<mutex> lock(queueMutex);
                processes.push(newProc);
                sort_queue(processes);
            }
        }

        cout << "[Process " << name << " added dynamically]\n" << overall_time <<"\n";
    }
}

void roundRobin(queue<Processes>& processes, float quantum, bool live) {

    queue<vector<float>> time_slots;  // to store start and end time of process execution for Gantt chart
    queue<char> operate;
    queue<Processes> terminatedProcesses;
    Processes operating;

    // Start input thread
    thread inputThread(dynamicInput, ref(processes), ref(queueMutex), ref(stopInput));

    while (!readyQueue.empty() || !processes.empty() || !stopInput) {
        // Move processes to readyQueue based on arrival times
        {
            lock_guard<mutex> lock(queueMutex);
            while (!processes.empty() && processes.front().getArrival() <= overall_time) {
                readyQueue.push(processes.front());
                processes.pop();
            }
        }

        if (readyQueue.empty()) {
            // Advance time to the next process arrival if readyQueue is empty
            {
                lock_guard<mutex> lock(queueMutex);
                if (!processes.empty()) {
                    overall_time = processes.front().getArrival();
                }
                else if (stopInput) { // Exit if input has stopped and no processes remain
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

        // Calculate time slice
        float time_slice = min(quantum, operating.getRemaining());
        operate.push(operating.getName());
        time_slots.push({ overall_time, overall_time + time_slice });
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

        if(live)wait(time_slice);
    }

    // Stop dynamic input thread
    stopInput = true;
    inputThread.detach();       //inputThread.join();

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
