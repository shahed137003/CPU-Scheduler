/********************************************************************************
** Form generated from reading UI file 'processinfo.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSINFO_H
#define UI_PROCESSINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_processInfo
{
public:
    QLabel *title;
    QLabel *Algorithm_label;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QDialog *processInfo)
    {
        if (processInfo->objectName().isEmpty())
            processInfo->setObjectName("processInfo");
        processInfo->resize(1300, 609);
        title = new QLabel(processInfo);
        title->setObjectName("title");
        title->setGeometry(QRect(0, 0, 1301, 81));
        Algorithm_label = new QLabel(processInfo);
        Algorithm_label->setObjectName("Algorithm_label");
        Algorithm_label->setGeometry(QRect(20, 100, 421, 31));
        label = new QLabel(processInfo);
        label->setObjectName("label");
        label->setGeometry(QRect(370, 240, 201, 20));
        label_2 = new QLabel(processInfo);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(370, 280, 191, 20));
        label_3 = new QLabel(processInfo);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(370, 320, 191, 20));
        label_4 = new QLabel(processInfo);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(370, 360, 181, 20));
        lineEdit = new QLineEdit(processInfo);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(640, 240, 301, 26));
        lineEdit_2 = new QLineEdit(processInfo);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(640, 280, 301, 26));
        lineEdit_3 = new QLineEdit(processInfo);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(640, 320, 301, 26));
        lineEdit_4 = new QLineEdit(processInfo);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(640, 360, 301, 26));
        pushButton = new QPushButton(processInfo);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(530, 420, 211, 51));
        label_5 = new QLabel(processInfo);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(370, 180, 561, 41));
        label_6 = new QLabel(processInfo);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(370, 160, 591, 101));

        retranslateUi(processInfo);

        QMetaObject::connectSlotsByName(processInfo);
    } // setupUi

    void retranslateUi(QDialog *processInfo)
    {
        processInfo->setWindowTitle(QCoreApplication::translate("processInfo", "Dialog", nullptr));
        title->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        Algorithm_label->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        label_4->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("processInfo", "PushButton", nullptr));
        label_5->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class processInfo: public Ui_processInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSINFO_H
