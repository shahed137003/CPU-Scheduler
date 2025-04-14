#include "GanttChart.h"

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
