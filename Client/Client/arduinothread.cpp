#include "arduinothread.h"
#include "mainwindow.h"
#include <QtCore>
ArduinoThread::ArduinoThread(QObject *parent):QThread(parent)
{
    this->alive=false;
}

void ArduinoThread::run(){

    alive=true;
    while(isAlive()){
        // emit the signal for the count label

        std::cerr<<"Arduino::run in"<<endl;
        emit startArdThread();
        // slowdown the count change, msec
        this->msleep(50);
        std::cerr<<"Arduino::run out"<<endl;
    }
    std::cerr<<"ArduinoThread closed"<<endl;
}

bool ArduinoThread::isAlive(){
    bool res=false;
    mutex.lock();
    std::cerr<<"Arduino::isAlive in"<<endl;
    res=alive;
    mutex.unlock();
    std::cerr<<"Arduino::isAlive out"<<endl;
    return res;
}

void ArduinoThread::terminate(){
    mutex.lock();
    std::cerr<<"Arduino::termiante in"<<endl;
    alive=false;
    mutex.unlock();
    std::cerr<<"Arduino::termiante out"<<endl;
}
