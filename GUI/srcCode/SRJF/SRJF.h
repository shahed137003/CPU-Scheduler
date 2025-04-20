#ifndef SRJF_H
#define SRJF_H

#include <QObject>
#include <QTimer>
#include <queue>
#include <vector>
#include <functional>
#include <thread>
#include "../Processes/Processes.h"
#include "GanttChart.h"
#include <atomic>



class SRJF : public QObject {
    Q_OBJECT



public:
    struct Process {
        int pid;
        int arrival_time;
        int burst_time;
        int remaining_time;
        int start_time;
        int finish_time;
        bool is_initial;
        char name;

        Process(int id, int arrival, int burst, bool initial,char nam)
            : pid(id), arrival_time(arrival), burst_time(burst),
            remaining_time(burst), start_time(-1), finish_time(-1),
            is_initial(initial),name(nam) {}
    };
    explicit SRJF(QObject *parent = nullptr);

    void runAlgo(std::queue<Process>& processes, bool live, float& current_time,GanttChart* gantt,std::mutex& mtx);

    QString printResults();
    QString calculateAverages(const std::vector<Process>& processes);
private:
    std::queue<Process> processes;
    std::priority_queue<Process*, std::vector<Process*>, std::function<bool(Process*, Process*)>> ready_queue;
    Process* current_process;
    float current_time;
    bool live;
    GanttChart* gantt;
    QTimer timer;
    bool initial_only;
    std::queue<char> operate; // For Gantt chart process names
    std::queue<std::vector<float>> time_slots; // For Gantt chart time intervals
    mutex mtx;
    queue<Process> new_processes;
    atomic<bool> stop_flag;
    atomic<bool> new_processes_added;
    QString results;

    //void calculateAverages(const std::vector<Process>& processes);
};

#endif
