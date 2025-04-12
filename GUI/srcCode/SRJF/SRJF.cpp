#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time = -1;
    int completion_time;
};


// HELPER FUNCTION FOR SORTING PROCESSES BY ARRIVAL TIME

bool compareArrival(const Process& a, const Process& b) {
    return a.arrival_time < b.arrival_time;
}



void srjfScheduler(vector<Process>& processes) {
    int n = processes.size();
    vector<Process> completed_processes;

    auto cmp = [](Process* a, Process* b) {
        return a->remaining_time > b->remaining_time;
        };

    priority_queue<Process*, vector<Process*>, decltype(cmp)> pq(cmp);
     // USES PRIORITY QUEUE TO MANGE PROCESSES BASED ON THIER REMANING TIME
    // (cmp) CUSTOM COMPARARTOR TO KEEP THE PROCESS WITH LEAST REMANING TIME AT THE TOP


    int current_time = 0;
    int index = 0;
    int completed = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;

    while (completed < n) {
        // Add newly arrived processes to the priority queue
        while (index < n && processes[index].arrival_time <= current_time) {
            pq.push(&processes[index]);
            index++;
        }

        if (!pq.empty()) {
            Process* current = pq.top();
            pq.pop();

            if (current->start_time == -1)
                current->start_time = current_time;

            current->remaining_time--;
            current_time++;

            if (current->remaining_time == 0) {
                current->completion_time = current_time;
                completed++;
                completed_processes.push_back(*current);
            }
            else {
                pq.push(current);
            }
        }
        else {
            // Idle time, no process is ready
            current_time++;
        }


        // NEED TO BE CHECKED !?
        // Allow dynamic process insertion
        char choice;
        cout << "Do you want to add a new process at time " << current_time << "? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            Process new_process;
            new_process.pid = processes.size() + 1;
            cout << "Enter arrival time: ";
            cin >> new_process.arrival_time;
            cout << "Enter burst time: ";
            cin >> new_process.burst_time;
            new_process.remaining_time = new_process.burst_time;

            processes.push_back(new_process);
            n++;  // Update number of total processes
            sort(processes.begin(), processes.end(), compareArrival);
        }
    }

    // Calculate waiting time and turnaround time
    for (const auto& p : completed_processes) {
        int turnaround_time = p.completion_time - p.arrival_time;
        int waiting_time = turnaround_time - p.burst_time;
        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;
    }

    cout << "\nAll processes completed.\n";
    cout << "Average Waiting Time: " << total_waiting_time / completed_processes.size() << endl;
    cout << "Average Turnaround Time: " << total_turnaround_time / completed_processes.size() << endl;
}

int main() {
    string scheduler_type;
    int n;

    cout << "Enter Scheduler Type (SRJF): ";
    cin >> scheduler_type;

    if (scheduler_type != "SRJF") {
        cout << "Unsupported Scheduler Type.\n";
        return 0;
    }

    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time for process " << i + 1 << ": ";
        cin >> processes[i].arrival_time;
        cout << "Enter burst time for process " << i + 1 << ": ";
        cin >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    sort(processes.begin(), processes.end(), compareArrival);
    srjfScheduler(processes);

    return 0;
}
