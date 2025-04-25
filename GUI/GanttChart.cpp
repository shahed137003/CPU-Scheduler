#include "GanttChart.h"
#include <QPainter>
#include <QDebug>

GanttChart::GanttChart(QWidget *parent) : QWidget(parent), isLiveMode(false) {
    connect(this, &GanttChart::requestUpdateGantt, this, &GanttChart::handleUpdateGantt);
}

void GanttChart::updateGanttChart(std::queue<char>& processes, std::queue<std::vector<float>>& timeSlots, bool live) {
    // Emit signal to ensure update happens in the main thread
    qDebug() << "GanttChart::updateGanttChart - processes size:" << processes.size()
             << "timeSlots size:" << timeSlots.size();
    emit requestUpdateGantt(processes, timeSlots, live);
}

void GanttChart::handleUpdateGantt(std::queue<char>& processes, std::queue<std::vector<float>>& timeSlots, bool live) {
    int originalProcessCount = processes.size(); // Store original count
    float expectedStart = 0.0f; // Tracks where the next process should start

    for (int i = 0; i < originalProcessCount; i++) {
        float processStart = timeSlots.front()[0];
        float processEnd = timeSlots.front()[1];

        // Check for a gap before this process
        if (processStart > expectedStart) {
            processes.push('#'); // Insert idle slot
            timeSlots.push({expectedStart, processStart});
        }

        // Move this process to the back of the queue
        processes.push(processes.front());
        timeSlots.push(timeSlots.front());
        processes.pop();
        timeSlots.pop();

        expectedStart = processEnd; // Update expected next start time
    }
    // Clear existing data
    while (!processNames.empty()) processNames.pop();
    while (!timeIntervals.empty()) timeIntervals.pop();
    qDebug()<< processes.size();
    processNames = processes;
    timeIntervals = timeSlots;
    isLiveMode = live;

    qDebug() << "handleUpdateGantt: Stored" << processNames.size() << "processes";
    update(); // Trigger repaint
}
void GanttChart::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.fillRect(rect(), Qt::white);

    qDebug() << "GanttChart::paintEvent - processNames size:" << processNames.size()
             << "timeIntervals size:" << timeIntervals.size();

    if (processNames.empty() || timeIntervals.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "No processes scheduled.");
        return;
    }

    // Create temporary copies to avoid modifying originals
    auto tempNames = processNames;
    auto tempSlots = timeIntervals;

    // Calculate total time duration
    float totalTime = 0;
    while (!tempSlots.empty()) {
        totalTime = std::max(totalTime, tempSlots.front()[1]); // Max end time
        tempSlots.pop();
    }
    tempSlots = timeIntervals; // Reset tempSlots for drawing

    qDebug() << "Total time:" << totalTime;

    const int barHeight = 30;
    const int startY = 50;
    const int margin = 20;
    const int textOffset = 15;

    // Draw each process bar
    while (!tempNames.empty() && !tempSlots.empty()) {
        char process = tempNames.front();
        float startTime = tempSlots.front()[0];
        float endTime = tempSlots.front()[1];
        float duration = endTime - startTime;

        qDebug() << "Drawing process:" << process << "start:" << startTime << "end:" << endTime;

        // Calculate bar dimensions
        int barWidth = static_cast<int>((duration / totalTime) * (width() - 2 * margin));
        int barX = margin + static_cast<int>((startTime / totalTime) * (width() - 2 * margin));

        // Draw the bar
        QRect barRect(barX, startY, barWidth, barHeight);
        painter.setBrush(QColor(100, 150, 255));
        painter.drawRect(barRect);

        // Draw process name
        painter.drawText(barRect, Qt::AlignCenter, QString(process));

        // Draw start and end times
        painter.drawText(barX, startY + barHeight + textOffset, QString::number(startTime));
        if(tempSlots.size() == 1)
            painter.drawText(barX + barWidth - 15, startY + barHeight + textOffset, " " +QString::number(endTime));

        tempNames.pop();
        tempSlots.pop();
    }

    // Draw timeline
    painter.drawLine(margin, startY + barHeight + textOffset + 10, width() - margin, startY + barHeight + textOffset + 10);
}
