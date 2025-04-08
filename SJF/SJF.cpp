#include "SJF.h"
void printGantt(vector<Process> p)
{
    cout << "Gantt Chart:\n";
    for(int i = 0; i < p.size(); i++)
    {
        cout << p[i].name << " ";
        for(int j = 0; j < p[i].burst_time; j++)
            cout << "-";
        cout << " ";
    }
    cout << endl;
}
void calc_avg_turn_around_time(vector<Process> processes)
{
    float total=0;
    for(int i=0;i<processes.size();i++)
        total+=processes[i].turn_around_time;
    cout << "Average Turnaround Time: " << total / processes.size() << endl;
}
void calc_avg_waiting_time(vector<Process> processes)
{
    float total=0;
    for(int i=0;i<processes.size();i++)
        total+=processes[i].waiting_time;
    cout << "Average Waiting Time: " << total / processes.size() << endl;
}
void SJF(vector<Process> processes)
{
    vector<Process> ordered_processes;
    float current_time = 0;
    float total_waiting_time=0;
    float total_turn_around_time=0;
    while (!processes.empty())
    {
        // we must sort the processes based on arrival time and burst time
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            if (a.arrival_time == b.arrival_time)
                return a.burst_time < b.burst_time;
            return a.arrival_time < b.arrival_time;
        });

        // we need to find the process that has arrived and has the shortest burst time
        Process selected_process;
        bool process_found = false;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current_time) {
                selected_process = processes[i];
                processes.erase(processes.begin() + i);
                process_found = true;
                break;
            }
        }

        if (process_found) {
            ordered_processes.push_back(selected_process);
            current_time += selected_process.burst_time;
            selected_process.completion_time=current_time;
            selected_process.turn_around_time=selected_process.completion_time-selected_process.arrival_time;
            selected_process.waiting_time=selected_process.turn_around_time-selected_process.burst_time;
            ordered_processes[ordered_processes.size() - 1] = selected_process;
            total_turn_around_time+=selected_process.turn_around_time;
            total_waiting_time+=selected_process.waiting_time;
        }
        else
           current_time++; //If there is no process ready, increment the current time
    }

    printGantt(ordered_processes);
    calc_avg_turn_around_time(ordered_processes);
    calc_avg_waiting_time(ordered_processes);
}

int main()
{
    string done = "Y";
    string type;
    int i = 0;
    queue<Process> ready_queue;
    cout << "Enter Scheduler Type: ";
    cin >> type;
    while (done == "Y" || done == "y")
    {
        Process p;
        i++;
        cout << "Enter process " << i << " name: ";
        cin >> p.name;
        cout << "Enter process " << i << " arrival time: ";
        cin >> p.arrival_time;
        cout << "Enter process " << i << " burst time: ";
        cin >> p.burst_time;
        p.remaining_time = p.burst_time;
        ready_queue.push(p);
        cout << "Do you want to add more processes? (Y/N) ";
        cin >> done;
    }

    vector<Process> processes;
    while (!ready_queue.empty()) {
        processes.push_back(ready_queue.front());
        ready_queue.pop();
    }

    if(type=="SJF")
        SJF(processes);
    return 0;
}
