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
    std::queue<std::vector<float>> time_slots;
    float overall_time = 0;
    bool live;
    GanttChart* gantt;

    PriorityNon(std::vector<Processes>& initialProcesses, bool live, GanttChart* gantt, QObject* parent = nullptr);

    void start();

signals:
    void requestProcessStep();

public slots:
    void processStep();

private:
    void printResults();
};

#endif // PRIORITYNON_H
