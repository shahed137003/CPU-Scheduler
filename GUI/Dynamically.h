#ifndef DYNAMICALLY_H
#define DYNAMICALLY_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <queue>
#include <vector>

class GanttChart : public QWidget {
    Q_OBJECT
public:
    explicit GanttChart(QWidget *parent = nullptr);
    void updateGanttChart(std::queue<char> processes, std::queue<std::vector<float>> timeSlots, bool live = false);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::queue<char> processNames;
    std::queue<std::vector<float>> timeIntervals;
    bool isLiveMode;
};

class Dynamically : public QWidget {
    Q_OBJECT
public:
    explicit Dynamically(QWidget *parent = nullptr);

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
