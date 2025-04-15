#ifndef DYNAMICALLY_H
#define DYNAMICALLY_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <queue>
#include "GanttChart.h"
#include "srcCode/Processes/Processes.h" // Assuming Processes and Process are defined here
#include "srcCode/FCFS/FCFS.h"
#include "srcCode/PriorityNon/PriorityNon.h"
#include "srcCode/PriorityPre/PriorityPre.h"
#include "srcCode/RoundRobin/RoundRobin.h"
#include "srcCode/SJF_Non/SJF_Non.h"
#include "srcCode/SRJF/SRJF.h"

class Dynamically : public QWidget {
    Q_OBJECT
public:
    explicit Dynamically(QWidget *parent = nullptr);
    void callAlgo(std::vector<Processes> processes, std::vector<Processes> process, float quantum, int comboIndex);

private slots:
    void addProcess();

private:
    QLineEdit *processNameEdit;
    QLineEdit *arrivalTimeEdit;
    QLineEdit *burstTimeEdit;
    QPushButton *addButton;
    QTableWidget *processTable;
    GanttChart *ganttChart;
};

#endif // DYNAMICALLY_H
