#include "arduinothread.h"
#include "mainwindow.h"
#include <QtCore>
ArduinoThread::ArduinoThread(QObject *parent,bool b):QThread(parent),stop(b)
{

}

void ArduinoThread::run(){

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
            this->msleep(50);
        }

}
