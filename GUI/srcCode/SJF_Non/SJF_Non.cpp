#include "SJF_Non.h"
void SJF_Non(vector<Processes>& processes)
{
    queue<char> ordered_processes_with_names;
    queue<Processes> ordered_processes;
    float current_time = 0;
    queue<vector<float>>time_slots;
    while (!processes.empty())
    {
        // we must sort processes based on burst time first, and arrival time second if burst times are equal
        sort(processes.begin(), processes.end(),compareByBurst);

        // we need to find the process that has arrived and has the shortest burst time
        Processes selected_process;
        bool process_found = false;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].getArrival() <= current_time) {
                selected_process = processes[i];
                processes.erase(processes.begin() + i);
                process_found = true;
                break;
            }
        }

        if (process_found) {
            ordered_processes_with_names.push(selected_process.getName());
            ordered_processes.push(selected_process);
            time_slots.push({ current_time,current_time + selected_process.getBurst() });
            current_time += selected_process.getBurst();

        }
        else
           current_time++; //If there is no process ready, increment the current time
    }

    printGantt(ordered_processes_with_names,time_slots,true);
    calcAvg_turn_time(ordered_processes);
    calcAvg_wait_time(ordered_processes);
}
/*
int main()
{
    string done = "Y";
    string type;
    int i = 0;
    float burst,arrival;
    char name;
    queue<Processes> ready_queue;
    cout << "Enter Scheduler Type: ";
    cin >> type;
    while (done == "Y" || done == "y")
    {
        Processes p;
        i++;
        cout << "Enter process " << i << " name: ";
        cin >> name;
        p.setName(name);
        cout << "Enter process " << i << " arrival time: ";
        cin >> arrival;
        p.setArrival(arrival);
        cout << "Enter process " << i << " burst time: ";
        cin >> burst;
        p.setBurst(burst);
        p.setRemaining (burst);
        ready_queue.push(p);
        cout << "Do you want to add more processes? (Y/N) ";
        cin >> done;
    }

    vector<Processes> processes;
    while (!ready_queue.empty()) {
        processes.push_back(ready_queue.front());
        ready_queue.pop();
    }

    if(type=="SJF")
        SJF_Non(processes);
    return 0;
}*/
