#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QWidget>
#include <queue>
#include <vector>

class GanttChart : public QWidget {
    Q_OBJECT
public:
    explicit GanttChart(QWidget *parent = nullptr);
    void updateGanttChart(const std::queue<char>& processes, const std::queue<std::vector<float>>& timeSlots, bool live);
    std::queue<char> processNames;
    std::queue<std::vector<float>> timeIntervals;
    bool isLiveMode;

signals:
    void requestUpdateGantt(const std::queue<char>& processes, const std::queue<std::vector<float>>& timeSlots, bool live);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void handleUpdateGantt(const std::queue<char>& processes, const std::queue<std::vector<float>>& timeSlots, bool live);

private:
         //std::queue<char> processNames;
         //std::queue<std::vector<float>> timeIntervals;
         //bool isLiveMode;
};

#endif
