#include "dynamically.h"
#include <QHeaderView>
#include <algorithm>

// ==================== GanttChart Implementation ====================
GanttChart::GanttChart(QWidget *parent) : QWidget(parent), isLiveMode(false) {}

void GanttChart::updateGanttChart(std::queue<char> processes, std::queue<std::vector<float>> timeSlots, bool live) {
    processNames = processes;
    timeIntervals = timeSlots;
    isLiveMode = live;
    update(); // Trigger repaint
}

void GanttChart::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.fillRect(rect(), Qt::white);

    if (processNames.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "No processes scheduled.");
        return;
    }

    // Calculate total time duration
    float totalTime = 0;
    auto tempSlots = timeIntervals;
    while (!tempSlots.empty()) {
        totalTime = std::max(totalTime, tempSlots.front()[1]); // Max end time
        tempSlots.pop();
    }

    const int barHeight = 30;
    const int startY = 50;
    const int margin = 20;
    const int textOffset = 15;

    // Draw each process bar
    while (!processNames.empty() && !timeIntervals.empty()) {
        char process = processNames.front();
        float startTime = timeIntervals.front()[0];
        float endTime = timeIntervals.front()[1];
        float duration = endTime - startTime;

        // Calculate bar dimensions (scaled to fit available width)
        int barWidth = static_cast<int>((duration / totalTime) * (width() - 2 * margin));
        int barX = margin + static_cast<int>((startTime / totalTime) * (width() - 2 * margin));

        // Draw the bar
        QRect barRect(barX, startY, barWidth, barHeight);
        painter.setBrush(QColor(100, 150, 255));
        painter.drawRect(barRect);

        // Draw process name inside the bar
        painter.drawText(barRect, Qt::AlignCenter, QString(process));

        // Draw start and end times below the bar
        painter.drawText(barX, startY + barHeight + textOffset, QString::number(startTime, 'f', 1));
        painter.drawText(barX + barWidth - 15, startY + barHeight + textOffset, QString::number(endTime, 'f', 1));

        processNames.pop();
        timeIntervals.pop();
    }

    // Draw timeline
    painter.drawLine(margin, startY + barHeight + textOffset + 10, width() - margin, startY + barHeight + textOffset + 10);
}

// ==================== Dynamically Implementation ====================
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
