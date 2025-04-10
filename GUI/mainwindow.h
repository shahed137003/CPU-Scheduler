#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Processes_file.h"
#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QString>
#include <sstream>
#include <iostream>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QColor>
#include <QTextBlock>
#include <QFontMetrics>
#include <QMainWindow>
#include "processinfo.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Nextbutton_clicked();

private:
    Ui::MainWindow *ui;
    processInfo *processInfo;

};
#endif // MAINWINDOW_H
