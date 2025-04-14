QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += srcCode

SOURCES += \
    Dynamically.cpp \
    main.cpp \
    mainwindow.cpp \
    processinfo.cpp\
    srcCode/FCFS/FCFS.cpp \
    srcCode/PriorityNon/PriorityNon.cpp \
    srcCode/PriorityPre/PriorityPre.cpp \
    srcCode/Processes/Processes.cpp \
    srcCode/RoundRobin/RoundRobin.cpp \
    srcCode/SJF_Non/SJF_Non.cpp \
    srcCode/SRJF/SRJF.cpp

HEADERS += \
    Dynamically.h \
    mainwindow.h \
    processinfo.h \
    srcCode/FCFS/FCFS.h \
    srcCode/PriorityNon/PriorityNon.h \
    srcCode/PriorityPre/PriorityPre.h \
    srcCode/Processes/Processes.h \
    srcCode/RoundRobin/RoundRobin.h \
    srcCode/SJF_Non/SJF_Non.h \
    srcCode/SRJF/SRJF.h

FORMS += \
    mainwindow.ui \
    processinfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

