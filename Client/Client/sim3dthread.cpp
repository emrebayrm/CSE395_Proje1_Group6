#include "sim3dthread.h"

Sim3DThread::Sim3DThread(QObject *parent):QThread(parent)
{

}

void Sim3DThread::run(){

    alive=true;
    while(isAlive())
        {
            qDebug("Sim3DThread run in");
            emit startThread();

            // slowdown the count change, msec
            this->msleep(80);
            qDebug("Sim3DThread run out");
        }
    qDebug("Sim3DThread closed");
}
bool Sim3DThread::isAlive(){
    bool res=false;
    mutex.lock();
    qDebug("Sim3DThread::isAlive in");
    res=alive;
    mutex.unlock();
    qDebug("Sim3DThread::isAlive out");
    return res;
}

void Sim3DThread::terminate(){
    mutex.lock();
    qDebug("Sim3DThread::termiante in");
    alive=false;
    mutex.unlock();
    qDebug("Sim3DThread::termiante out");
}
