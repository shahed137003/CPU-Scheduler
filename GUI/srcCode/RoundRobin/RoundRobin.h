#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include <queue>
#include <mutex>
#include <atomic>
#include <QObject>
#include <QTimer>
#include "../Processes/Processes.h"
#include "GanttChart.h"

class RoundRobin : public QObject {
    Q_OBJECT
public:
    RoundRobin(std::queue<Processes>& processes, float quantum, bool live, GanttChart* gantt, QObject* parent = nullptr);

    void start();
    QString printResults();
signals:
    void requestProcessStep();
private slots:
    void processStep();

private:
    std::queue<Processes> processes;
    std::queue<Processes> readyQueue;
    std::queue<Processes> terminatedProcesses;
    std::queue<char> operate;
    std::queue<std::vector<float>> time_slots;
    float quantum;
    bool live;
    GanttChart* gantt;
    float overall_time;
    std::mutex queueMutex;
    std::atomic<bool> stopInput;
    QTimer timer;
    Processes operating;
    bool isOperating;
    QString results;

};

#endif
