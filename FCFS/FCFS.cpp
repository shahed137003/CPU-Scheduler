#include "FCFS.h"


bool compareArrival(Processes a, Processes b) {
    return a.getArrival() < b.getArrival();
}

void FCFS(vector<Processes>& processes) {
    sort(processes.begin(), processes.end(), compareArrival); // Sort by arrival

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
}

int main() {
    vector<Processes> processList;
    processList.push_back(Processes('A', 0, 5));
    processList.push_back(Processes('B', 1, 3));
    processList.push_back(Processes('C', 2, 8));
    processList.push_back(Processes('D', 3, 6));

    FCFS(processList);

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
}
