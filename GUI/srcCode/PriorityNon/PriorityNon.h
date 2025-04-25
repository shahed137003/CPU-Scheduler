#ifndef PRIORITYNON_H
#define PRIORITYNON_H

#include <QObject>
#include <queue>
#include <vector>
#include "../Processes/Processes.h"
#include "GanttChart.h"

class PriorityNon : public QObject {
    Q_OBJECT
public:
    std::vector<Processes> processes;
    std::queue<Processes> readyQueue;
    std::queue<Processes> terminatedProcesses;
    std::queue<char> operate;
    QString results;
    void runAlgo(std::vector<Processes>& initialProcesses, std::queue<std::pair<char,float>>&remaining, bool live, float& overall_time, GanttChart* gantt, std::mutex& vectorMutex);
    std::queue<std::vector<float>> time_slots;
    float overall_time = 0;
    bool live;
    GanttChart* gantt;
    explicit PriorityNon(QObject *parent = nullptr);
    QString printResults();




private:
};

#endif // PRIORITYNON_H
