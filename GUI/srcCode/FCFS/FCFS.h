#ifndef FCFS_H
#define FCFS_H

#include <QObject>
#include <queue>
#include <vector>
#include "../Processes/Processes.h"
#include "GanttChart.h"

class FCFS : public QObject {
    Q_OBJECT
public:
    explicit FCFS(QObject *parent = nullptr);
    void runAlgo(std::vector<Processes>& processes, std::queue<std::pair<char,float>>&remaining, bool live, float& overall_time,GanttChart* gantt,
                 std::mutex& allMutex);

    QString printResults();

private:
    std::queue<Processes> readyQueue;
    std::queue<Processes> terminatedProcesses;
    std::queue<char> operate;
    std::queue<std::vector<float>> time_slots;
    float overall_time;
    //QTimer timer;
    Processes operating;
    bool isOperating;
    QString results;

};


#endif // FCFS_H

