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
    explicit RoundRobin(QObject *parent = nullptr);
    void runAlgo(std::queue<Processes>& processes, float quantum, bool live, float& overall_time,GanttChart* gantt,
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

#endif
