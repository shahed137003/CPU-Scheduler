#include "Dynamically.h"
#include "srcCode/RoundRobin/RoundRobin.h"
#include <QHeaderView>
#include <algorithm>
#include <vector>
Dynamically::Dynamically(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Process Scheduler with Gantt Chart");
    resize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *inputWidget = new QWidget;
    QVBoxLayout *inputLayout = new QVBoxLayout(inputWidget);

    processNameEdit = new QLineEdit;
    processNameEdit->setPlaceholderText("Process Name (e.g., A)");
    arrivalTimeEdit = new QLineEdit;
    arrivalTimeEdit->setPlaceholderText("Arrival Time");
    burstTimeEdit = new QLineEdit;
    burstTimeEdit->setPlaceholderText("Burst Time");
    addButton = new QPushButton("Add Process");

    inputLayout->addWidget(new QLabel("Add New Process:"));
    inputLayout->addWidget(processNameEdit);
    inputLayout->addWidget(arrivalTimeEdit);
    inputLayout->addWidget(burstTimeEdit);
    inputLayout->addWidget(addButton);
    inputLayout->addStretch();

    processTable = new QTableWidget;
    processTable->setColumnCount(3);
    processTable->setHorizontalHeaderLabels({"Process", "Arrival", "Burst"});
    processTable->horizontalHeader()->setStretchLastSection(true);

    ganttChart = new GanttChart(this);

    mainLayout->addWidget(inputWidget, 1);
    mainLayout->addWidget(processTable, 2);
    mainLayout->addWidget(ganttChart, 3);

    connect(addButton, &QPushButton::clicked, this, &Dynamically::addProcess);
}

void Dynamically::addProcess() {
    // Implementation remains unchanged or update as needed
}

void Dynamically::callAlgo(std::vector<Processes> processes, std::vector<Processes> process, float quantum, int comboIndex) {
    switch (comboIndex) {
    case 0:
        //FCFS(processes);
        break;
    case 1:
        //SJF_Non(processes);
        break;
    case 2:
        //scheduleSRJF(process);
        break;
    case 3:
        //PriorityNon(processes);
        break;
    case 4:
        //PriorityPre(processes);
        break;
    case 5:
        std::queue<Processes> processesQ;
        for (const auto& proc : processes) {
            processesQ.push(proc);
        }
        bool live = true;
        roundRobin(processesQ, quantum, live, ganttChart);
        break;
    }
}
