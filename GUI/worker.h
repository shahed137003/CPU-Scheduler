#ifndef WORKER_H
#define WORKER_H
#include <QThread>
#include "srcCode/Processes/Processes.h"
#include "srcCode/SRJF/SRJF.h"
#include "Dynamically.h"
#include "timerthread.h"

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(std::vector<Processes>* processes,
           std::queue<Processes>* processesQ,
           std::queue<SRJF::Process>* process,
           float* overall_time,
           float quantum,
           int comboIndex,
           bool isLive,
           std::mutex* allMutex,
           QObject* parent = nullptr)
        : QObject(parent),
        processes_(processes),
        processesQ_(processesQ),
        process_(process),
        overall_time_(overall_time),
        quantum_(quantum),
        comboIndex_(comboIndex),
        isLive_(isLive),
        allMutex_(allMutex),
        dynamically_(nullptr) {}

    ~Worker() {
        if (dynamically_) {
            dynamically_->deleteLater();
        }
    }

    void startTimer() {
        m_timerThread = new TimerThread(this);
        connect(m_timerThread, &TimerThread::timeUpdated, this, &Worker::timeUpdated);
        m_timerThread->start();
    }

    void stopTimer() {
        if (m_timerThread) {
            m_timerThread->stop();
            m_timerThread->quit();
            m_timerThread->wait();
            delete m_timerThread;
            m_timerThread = nullptr;
        }
    }

public slots:
    void run() {
        if (!processes_ || !processesQ_ || !process_ || !allMutex_) {
            emit finished();
            return;
        }

        dynamically_ = new Dynamically(nullptr);
        dynamically_->show();

        // Connect a signal to know when callAlgo is done
        connect(dynamically_, &Dynamically::algorithmFinished, this, &Worker::onAlgorithmFinished);

        // Call the algorithm
        dynamically_->callAlgo(*processes_, *processesQ_, *process_, quantum_, comboIndex_, isLive_, *overall_time_, *allMutex_);
    }

    void onAlgorithmFinished() {
        emit finished(); // Signal completion only after algorithm finishes
    }

signals:
    void finished();
    void timeUpdated(float newTime);

private:
    std::vector<Processes>* processes_;
    std::queue<Processes>* processesQ_;
    std::queue<SRJF::Process>* process_;
    float* overall_time_;
    float quantum_;
    int comboIndex_;
    bool isLive_;
    std::mutex* allMutex_;
    TimerThread* m_timerThread = nullptr;
    Dynamically* dynamically_;
};
#endif // WORKER_H
