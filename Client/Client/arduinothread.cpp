#include "arduinothread.h"
#include "mainwindow.h"
#include <QtCore>
ArduinoThread::ArduinoThread(QObject *parent,bool b):QThread(parent),stop(b)
{

}

void ArduinoThread::run(){

            QMutex mutex;
            // prevent other threads from changing the "Stop" value
            mutex.lock();
            if(this->stop) return;
            mutex.unlock();

            // emit the signal for the count label
            emit startArdThread();

            // slowdown the count change, msec
            this->msleep(50);

}
