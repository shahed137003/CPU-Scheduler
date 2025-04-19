// timerthread.cpp
#include "timerthread.h"
#include <QElapsedTimer>

TimerThread::TimerThread(QObject *parent)
    : QThread(parent), m_running(true), m_currentTime(0.0f)
{
}

void TimerThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
}

void TimerThread::run()
{
    QElapsedTimer timer;
    timer.start();

    float time = 0.0;
    while (m_running) {
        emit timeUpdated(time);
        time += 0.1; // Example increment
        msleep(100); // Example delay
    }
}
