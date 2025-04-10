#include "PriorityPre.h"


void PriorityPre(vector<Processes>& processes)
{

    priority_queue<Processes,vector<Processes>,compareByPriority> pq;
    float total_burst=0;
    for(int i=0;i<processes.size();i++)
    {
        processes[i].setRemaining(processes[i].getBurst());
        pq.push(processes[i]);
        total_burst+=processes[i].getBurst();
    }

    for(float i=0;i<total_burst;i++){   

    for(auto& process:pq){ //const hna wala la

    if(process.getArrival()<=i&&process.getRemaining()!=0){
       process.setRemaining(process.getRemaining()-1);
       if(process.getRemaining()==0&&process.getLasttime()!=1)
       {
        process.setLasttime(1);
        float arr = process.getArrival();
        float burst = process.getBurst();
        float turnaround= i-arr;
        float waiting=turnaround-burst;
        process.setTurnaround(turnaround);
        process.setWaiting(waiting);
       }
       break();
    }
   
    }

    }

 





}




   


int main() {
    vector<Processes> processList;
    processList.push_back(Processes('A', 0, 5, 0));
    processList.push_back(Processes('B', 1, 3, 1));
    processList.push_back(Processes('C', 2, 8, 2));
    processList.push_back(Processes('D', 3, 6, 3));

    PriorityPre(processList);

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



















