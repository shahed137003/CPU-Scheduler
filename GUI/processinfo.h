#ifndef PROCESSINFO_H
#define PROCESSINFO_H
#include "srcCode/Processes/Processes.h"
#include "srcCode/FCFS/FCFS.h"
#include "srcCode/PriorityNon/PriorityNon.h"
#include "srcCode/PriorityPre/PriorityPre.h"
#include "srcCode/RoundRobin/RoundRobin.h"
#include "srcCode/SJF_Non/SJF_Non.h"
#include "srcCode/SRJF/SRJF.h"
#include "Dynamically.h"
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QString>
namespace Ui {
class processInfo;
}

class processInfo : public QDialog
{
    Q_OBJECT

public:
    explicit processInfo(QWidget *parent = nullptr);
    ~processInfo();
    void receiveProcessData(QString algorithm, int numProcesses, int index,bool live);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::processInfo *ui;
    Dynamically *dynamically;
    int processNum;
    QString AlgorithmNum;
};

#endif // PROCESSINFO_H
