#ifndef PRIORITYPRE_H
#define PRIORITYPRE_H

#include <QObject>
#include <queue>
#include <vector>
#include "../Processes/Processes.h"
#include "GanttChart.h"

class PriorityPre : public QObject {
    Q_OBJECT
public:
    std::vector<Processes> processes;
    std::queue<Processes> readyQueue;
    std::queue<Processes> terminatedProcesses;
    std::queue<char> operate;
    std::queue<std::vector<float>> time_slots;
    float overall_time = 0;
    bool live;
    GanttChart* gantt;
    QString results;
    explicit PriorityPre(QObject *parent = nullptr);
    void runAlgo(std::vector<Processes>& initialProcesses, bool live, float& overall_time, GanttChart* gantt, std::mutex& vectorMutex);
    QString printResults();
    char last;



private:
};

#endif // PRIORITYPRE_H
