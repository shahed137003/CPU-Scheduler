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
#include "srcCode/SRJF/SRJF.h"

class Dynamically : public QWidget {
    Q_OBJECT
public:
    explicit Dynamically(QWidget *parent = nullptr);
    void callAlgo(std::vector<Processes> processes, std::vector<Process> process, float quantum, int comboIndex);

private slots:
    void addProcess();

private:
    QLineEdit *processNameEdit;
    QLineEdit *arrivalTimeEdit;
    QLineEdit *burstTimeEdit;
    QPushButton *addButton;
    QTableWidget *processTable;
    GanttChart *ganttChart;

    // Scheduling algorithms
    void FCFS(std::vector<Processes> processes);
    void SJF_Non(std::vector<Processes> processes); // Corrected declaration
    void scheduleSRJF(std::vector<Process> process);
    void PriorityNon(std::vector<Processes> processes);
    void PriorityPre(std::vector<Processes> processes);
};

#endif // DYNAMICALLY_H
