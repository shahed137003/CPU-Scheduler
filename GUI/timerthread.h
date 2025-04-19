// timerthread.h
#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QMutex>

class TimerThread : public QThread
{
    Q_OBJECT
public:
    explicit TimerThread(QObject *parent = nullptr);
    void stop();

signals:
    void timeUpdated(float currentTime);

protected:
    void run() override;

private:
    bool m_running;
    QMutex m_mutex;
    float m_currentTime;
};

#endif // TIMERTHREAD_H
