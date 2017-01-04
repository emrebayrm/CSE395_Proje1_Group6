#ifndef SIM3DTHREAD_H
#define SIM3DTHREAD_H
#include <QThread>
#include <QProcess>
#include <QMutex>
#include "threedsimulationmessage.h"

#define DEFAULTPORT 9999

class Sim3DThread : public QThread
{
    Q_OBJECT
public:
    explicit Sim3DThread(QObject* parent=0);
    void run();
    bool isAlive();
    void terminate();
    ThreeDSimulationMessage msg;
    int* connsock;
    QMutex *mtx;
    int connfd;
signals:
    void startThread();
    void Request(int);
public slots:
    void SendData(int,int,int,int);
private:
    QProcess *proc3D = NULL;
    bool alive;
    bool connectionCompleted;
};

#endif // SIM3DTHREAD_H
