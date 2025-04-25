#ifndef SJF_NON_H
#define SJF_NON_H

#include <QObject>
#include <queue>
#include <vector>
#include "../Processes/Processes.h"
#include "GanttChart.h"

class SJF_Non : public QObject {
    Q_OBJECT
public:
    explicit SJF_Non(QObject *parent = nullptr);
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

#endif // SJF_NON_H
