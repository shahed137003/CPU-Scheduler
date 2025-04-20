#include "Dynamically.h"
#include "srcCode/RoundRobin/RoundRobin.h"
#include <QHeaderView>
#include <algorithm>
#include <QApplication>



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
    resultsDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum); // Allow vertical expansion
    resultsDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable vertical scrollbar
    resultsDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scrollbar
    resultsDisplay->setLineWrapMode(QTextEdit::NoWrap); // Prevent line wrapping
    outputLayout->addWidget(new QLabel("Scheduling Results:"));
    outputLayout->addWidget(resultsDisplay);


    // ===== Slot 2: Gantt Chart =====
    ganttChart = new GanttChart;

    // Add all slots to main layout
    mainLayout->addWidget(outputWidget);
    mainLayout->addWidget(ganttChart);

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
void Dynamically::callAlgo(std::vector<Processes>& processes,
                           std::queue<Processes>& processesQ,
                           std::queue<SRJF::Process>& process,
                           float quantum,
                           int comboIndex,
                           bool live,
                           float &overall_time,
                           std::mutex& allMutex) {
    switch (comboIndex) {
        case 0: {
            FCFS* fcfs = new FCFS(nullptr);
            fcfs->runAlgo(processes, live, overall_time, ganttChart, allMutex);
            resultsDisplay->setText(fcfs->printResults());
            break;
        }
        case 1: {
            SJF_Non sjfnon(processes, live, ganttChart);
            sjfnon.start();
            resultsDisplay->setText(sjfnon.printResults());
            break;
        }
        case 2: {
            SRJF* srjf = new SRJF(nullptr);
            srjf->runAlgo(process, live, overall_time, ganttChart, allMutex);
            resultsDisplay->setText(srjf->printResults());
            delete srjf; // Clean up to avoid memory leak
            break;
        }
        case 3: {
            PriorityNon pn(processes, live, ganttChart);
            pn.start();
            resultsDisplay->setText(pn.printResults());
            break;
        }
        case 4: {
            PriorityPre* PrioPre = new PriorityPre(nullptr);
            PrioPre->runAlgo(processes,live,overall_time, ganttChart, allMutex);
            resultsDisplay->setText(PrioPre->printResults());
            delete PrioPre; // Clean up to avoid memory leak
            break;
        }
        case 5: {
            qDebug() << "Calling roundRobin with quantum =" << quantum << ", live =" << live;
            RoundRobin* RRSARA = new RoundRobin(nullptr);
            RRSARA->runAlgo(processesQ, quantum, live, overall_time, ganttChart, allMutex);
            resultsDisplay->setText(RRSARA->printResults());
            delete RRSARA; // Clean up to avoid memory leak
            break;
        }
    }

    // Ensure GUI updates are processed
    QApplication::processEvents();
    wait(10);
    // Emit signal to indicate algorithm completion
    emit algorithmFinished();
}
