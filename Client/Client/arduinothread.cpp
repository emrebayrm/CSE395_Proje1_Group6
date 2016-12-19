#include "arduinothread.h"
#include "mainwindow.h"
#include <QtCore>
ArduinoThread::ArduinoThread(QObject *parent,bool b):QThread(parent),stop(b)
{

}

void ArduinoThread::run(){

    stop=false;
    QMutex mutex;
    while(!stop){
        mutex.lock();
        // prevent other threads from changing the "Stop" value

        // emit the signal for the count label
        startArdThread();

        // slowdown the count change, msec
        this->msleep(50);
        mutex.unlock();
    }

}
