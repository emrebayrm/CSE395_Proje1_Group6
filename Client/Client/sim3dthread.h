#ifndef SIM3DTHREAD_H
#define SIM3DTHREAD_H
#include <QThread>
#include"requirements.h"

class Sim3DThread : public QThread
{
    Q_OBJECT
public:
    explicit Sim3DThread(QObject* parent=0);
    void run();
    bool isAlive();
    void terminate();
    ArduinoMessageBean msg;

signals:
    void startThread();
public slots:
private:
    QMutex mutex;
    bool alive;
};

#endif // SIM3DTHREAD_H
