/********************************************************************************
** Form generated from reading UI file 'processinfo.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSINFO_H
#define UI_PROCESSINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_processInfo
{
public:
    QLabel *title;
    QLabel *Algorithm_label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *processInputsLayout;

    void setupUi(QDialog *processInfo)
    {
        if (processInfo->objectName().isEmpty())
            processInfo->setObjectName("processInfo");
        processInfo->resize(1300, 609);
        title = new QLabel(processInfo);
        title->setObjectName("title");
        title->setGeometry(QRect(320, 10, 631, 61));
        Algorithm_label = new QLabel(processInfo);
        Algorithm_label->setObjectName("Algorithm_label");
        Algorithm_label->setGeometry(QRect(10, 89, 421, 31));
        verticalLayoutWidget = new QWidget(processInfo);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 120, 1271, 441));
        processInputsLayout = new QVBoxLayout(verticalLayoutWidget);
        processInputsLayout->setObjectName("processInputsLayout");
        processInputsLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(processInfo);

        QMetaObject::connectSlotsByName(processInfo);
    } // setupUi

    void retranslateUi(QDialog *processInfo)
    {
        processInfo->setWindowTitle(QCoreApplication::translate("processInfo", "Dialog", nullptr));
        title->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
        Algorithm_label->setText(QCoreApplication::translate("processInfo", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class processInfo: public Ui_processInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSINFO_H
