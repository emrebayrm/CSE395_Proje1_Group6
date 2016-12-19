#include "sim3dthread.h"

Sim3DThread::Sim3DThread(QObject *parent,bool b):QThread(parent),stop(b)
{

}

void Sim3DThread::run(){

    while(1)
        {
            QMutex mutex;
            // prevent other threads from changing the "Stop" value
            mutex.lock();
            if(this->stop) break;
            mutex.unlock();

            // emit the signal for the count label
            emit startThread();

            // slowdown the count change, msec
            this->msleep(80);
        }

}
