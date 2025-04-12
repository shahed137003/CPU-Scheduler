#ifndef PROCESSINFO_H
#define PROCESSINFO_H
#include "Processes/Processes.h"
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
    void receiveProcessData(QString algorithm, int numProcesses);

private slots:
    void on_pushButton_clicked();

private:
    Ui::processInfo *ui;
    int processNum;
    QString AlgorithmNum;
};

#endif // PROCESSINFO_H
