#include "Dynamically.h"
#include "srcCode/RoundRobin/RoundRobin.h"
#include <QHeaderView>
#include <algorithm>



Dynamically::Dynamically(QWidget *parent) : QWidget(parent) {

    setWindowTitle("Process Scheduler with Gantt Chart");
    resize(900, 400);

    // Main layout (2 vertical slots)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ===== Slot 1: Output Fields =====
    outputWidget = new QWidget;
    QVBoxLayout *outputLayout = new QVBoxLayout(outputWidget);

    // Add a QTextEdit for displaying results
    resultsDisplay = new QTextEdit;
    resultsDisplay->setReadOnly(true); // Make it read-only
    outputLayout->addWidget(new QLabel("Scheduling Results:"));
    outputLayout->addWidget(resultsDisplay);


    // ===== Slot 2: Gantt Chart =====
    ganttChart = new GanttChart;

    // Add all slots to main layout
    mainLayout->addWidget(outputWidget, 1);
    mainLayout->addWidget(ganttChart, 2);

    // Connect button to addProcess()
    //connect(addButton, &QPushButton::clicked, this, &Dynamically::addProcess);
}

void Dynamically::addProcess() {
    /*QString name = processNameEdit->text();
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
    burstTimeEdit->clear();*/
}
void Dynamically::callAlgo(std::vector<Processes> processes, std::vector<SRJF::Process> process, float quantum, int comboIndex, bool live) {
    std::queue<Processes> processesQ;
    // Use input processes or hardcoded ones for testing
    /*if (true) {
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
    }
    else {

    }*/
    for (const auto& proc : processes) {
        processesQ.push(proc);
        qDebug() << "Adding process to queue:" << proc.getName();
    }
    qDebug() << comboIndex;

    std::queue<char> processes_name;
    std::queue<std::vector<float>> timeSlots;
    switch (comboIndex) {
    case 0:{
        FCFS fcfs(processes,live,ganttChart);
        fcfs.start();
        resultsDisplay->setText(fcfs.printResults());
        break;
    }

    case 1:{
        SJF_Non sjfnon(processes,live,ganttChart);
        sjfnon.start();
        resultsDisplay->setText(sjfnon.printResults());
        break;
    }

    case 2:{
        // scheduleSRJF(process);
       SRJF srjf(
            process,        // std::vector<Process>& initialProcesses
            live,        // bool live
            ganttChart,      // GanttChart* gantt
            nullptr          // QObject* parent (optional if default is nullptr)
            );

        srjf.start();  // Begins the scheduling process
        qDebug()<<processes_name.size();
        resultsDisplay->setText(srjf.printResults());
        //ganttChart->updateGanttChart(processes_name, timeSlots, false);
        break;
    }
    case 3:{
        PriorityNon pn(processes,live,ganttChart);
        pn.start();
        resultsDisplay->setText(pn.printResults());
        break;
    }
    case 4:{
        PriorityPre pp(processes,live,ganttChart);
        pp.start();
        resultsDisplay->setText(pp.printResults());
        break;
    }
    case 5:{
        qDebug() << "Calling roundRobin with quantum =" << quantum << ", live =" << live;
        RoundRobin RRSARA(processesQ, quantum, live, ganttChart);
        RRSARA.start();
        resultsDisplay->setText(RRSARA.printResults());
        //ganttChart->updateGanttChart(processes_name, timeSlots, false);
        break;

    }
    }
}
