#include "PriorityPre.h"


void PriorityPre(vector<Processes>& processes)
{
    float total_burst = 0;
    for (auto& process : processes) {
        process.setRemaining(process.getBurst());
        total_burst += process.getBurst();
    }

    for (float i = 0; i < total_burst; i++) {
        int selected = -1;
        for (int j = 0; j < processes.size(); j++) {
            if (processes[j].getArrival() <= i && processes[j].getRemaining() > 0) {
                if (selected == -1 || 
                    processes[j].getPriority() < processes[selected].getPriority() ||
                    (processes[j].getPriority() == processes[selected].getPriority() && 
                     processes[j].getArrival() < processes[selected].getArrival())) {
                    selected = j;
                }
            }
        }

        if (selected != -1) {
            Processes& process = processes[selected];
            process.setRemaining(process.getRemaining() - 1);

            if (process.getRemaining() == 0 && process.getLasttime() == -1) {
                float arr = process.getArrival();
                float burst = process.getBurst();
                float turnaround = i+1 - arr;
                float waiting = turnaround - burst;
                process.setLasttime(i+1);
                process.setTurnaround(turnaround);
                process.setWaiting(waiting);
            }
        }
    }
    queue<Processes> QueueOfProcesses;
    for (const auto& p : processes) {
        QueueOfProcesses.push(p);
    }
    cout << "Total Turnaround Time: " << calcTotal_turn_time(QueueOfProcesses) << "\n";
    cout << "Average Turnaround Time: " << calcAvg_turn_time(QueueOfProcesses) << "\n\n";
    cout << "Total Waiting Time: " << calcTotal_wait_time(QueueOfProcesses) << "\n";
    cout << "Average Waiting Time: " << calcAvg_wait_time(QueueOfProcesses) << "\n";
}



    

/*
int main() {
    vector<Processes> processList;
    processList.push_back(Processes('A', 0, 5, 2)); 
    processList.push_back(Processes('B', 1, 3, 0));
    processList.push_back(Processes('C', 2, 8, 0));
    processList.push_back(Processes('D', 3, 6, 1));

    PriorityPre(processList);

    cout << "Name\tArrival\tBurst\tPriority\tCompletion\tTurnaround\tWaiting\n";
    for (const auto& p : processList) {
        cout << p.getName() << "\t"
             << p.getArrival() << "\t"
             << p.getBurst() << "\t"
             <<p.getPriority()<<"\t\t"
             << p.getLasttime() << "\t\t"
             << p.getTurnaround() << "\t\t"
             << p.getWaiting() << endl;
    }

    return 0;
}
*/


















