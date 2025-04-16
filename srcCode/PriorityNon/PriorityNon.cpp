#include "PriorityNon.h"

void PriorityNon(vector<Processes>& processes) {
    sort(processes.begin(), processes.end(),compareByPriority); // Sort by priority

    float currentTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].getArrival() > currentTime)
            currentTime = processes[i].getArrival();

        currentTime += processes[i].getBurst();
        processes[i].setLasttime(currentTime);

        float turnaround = currentTime - processes[i].getArrival();
        processes[i].setTurnaround(turnaround);

        float waiting = turnaround - processes[i].getBurst();
        processes[i].setWaiting(waiting);
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

/*int main() {
    vector<Processes> processList;
    processList.push_back(Processes('A', 0, 5, 0));
    processList.push_back(Processes('B', 1, 3, 1));
    processList.push_back(Processes('C', 2, 8, 2));
    processList.push_back(Processes('D', 3, 6, 3));

    PriorityNon(processList);

    cout << "Name\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for (const auto& p : processList) {
        cout << p.getName() << "\t"
             << p.getArrival() << "\t"
             << p.getBurst() << "\t"
             << p.getLasttime() << "\t\t"
             << p.getTurnaround() << "\t\t"
             << p.getWaiting() << endl;
    }

    return 0;
}*/
