#include "Dynamically.h"
#include "srcCode/RoundRobin/RoundRobin.h"
#include <QHeaderView>
#include <algorithm>



Dynamically::Dynamically(QWidget *parent) : QWidget(parent) {

    setWindowTitle("Process Scheduler with Gantt Chart");
    resize(900, 600);

    // Main layout (3 horizontal slots)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ===== Slot 1: Input Fields =====
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

    // ===== Slot 2: Process Table =====
    processTable = new QTableWidget;
    processTable->setColumnCount(3);
    processTable->setHorizontalHeaderLabels({"Process", "Arrival", "Burst"});
    processTable->horizontalHeader()->setStretchLastSection(true);

    // ===== Slot 3: Gantt Chart =====
    ganttChart = new GanttChart;

    // Add all slots to main layout
    mainLayout->addWidget(inputWidget, 1);
    mainLayout->addWidget(processTable, 2);
    mainLayout->addWidget(ganttChart, 3);

    // Connect button to addProcess()
    connect(addButton, &QPushButton::clicked, this, &Dynamically::addProcess);
}

void Dynamically::addProcess() {
    QString name = processNameEdit->text();
    QString arrival = arrivalTimeEdit->text();
    QString burst = burstTimeEdit->text();

    if (name.isEmpty() || arrival.isEmpty() || burst.isEmpty()) {
        return; // Input validation
    }

    // Add to table
    int row = processTable->rowCount();
    processTable->insertRow(row);
    processTable->setItem(row, 0, new QTableWidgetItem(name));
    processTable->setItem(row, 1, new QTableWidgetItem(arrival));
    processTable->setItem(row, 2, new QTableWidgetItem(burst));

    // Generate a simple FCFS Gantt chart (for demo)
    std::queue<char> processes;
    std::queue<std::vector<float>> timeSlots;

    float currentTime = 0;
    for (int i = 0; i < processTable->rowCount(); ++i) {
        char procName = processTable->item(i, 0)->text().at(0).toLatin1();
        float burstTime = processTable->item(i, 2)->text().toFloat();

        processes.push(procName);
        timeSlots.push({currentTime, currentTime + burstTime});
        currentTime += burstTime;
    }

    ganttChart->updateGanttChart(processes, timeSlots, false);

    // Clear input fields
    processNameEdit->clear();
    arrivalTimeEdit->clear();
    burstTimeEdit->clear();
}
void Dynamically::callAlgo(std::vector<Processes> processes, std::vector<Process> process, float quantum, int comboIndex, bool live) {
    std::queue<Processes> processesQ;
    // Use input processes or hardcoded ones for testing
    if (true) {
        // Hardcoded for testing
        std::vector<Processes> processes_manual = {
            Processes('A', 0, 21),
            Processes('B', 0, 3),
            Processes('C', 0, 3)
        };
        for (const auto& proc : processes_manual) {
            processesQ.push(proc);
            qDebug() << "Adding process to queue:" << proc.getName();
        }
    }/*
    else {
        for (const auto& proc : processes) {
            processesQ.push(proc);
            qDebug() << "Adding process to queue:" << proc.getName();
        }
    }*/
    qDebug() << comboIndex;
    switch (comboIndex) {
    case 0:
        // FCFS(processes);
        break;
    case 1:
        // SJF_Non(processes);
        break;
    case 2:
        // scheduleSRJF(process);
        break;
    case 3:
        // PriorityNon(processes);
        break;
    case 4:
        // PriorityPre(processes);
        break;
    case 5:

        qDebug() << "Calling roundRobin with quantum =" << quantum << ", live =" << live;
        quantum = 2;
        RoundRobin(processesQ, quantum, live, ganttChart, true);
        break;
    }
}
