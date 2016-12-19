#include "graphicthread.h"
#include "mainwindow.h"
#include <QtCore>
GraphicThread::GraphicThread(QObject *parent):QThread(parent)
{
    this->alive=false;
}

void GraphicThread::run(){
    std::cerr<<"gui thread runn"<<endl;

    alive=true;
    while(isAlive()){
        // emit the signal for the count label
        std::cerr<<"Graphic:run in"<<endl;
        emit startThread();
        // slowdown the count change, msec
        this->msleep(80);
        std::cerr<<"Graphic:run out"<<endl;
    }
    std::cerr<<"GraphicThread closed"<<endl;
}

bool GraphicThread::isAlive(){
    bool res=false;
    mutex.lock();
    std::cerr<<"Graphic:isAlive in"<<endl;
    res=alive;
    mutex.unlock();
    std::cerr<<"Graphic:isAlive out"<<endl;
    return res;
}

void GraphicThread::terminate(){
    mutex.lock();
    std::cerr<<"Graphic:terminate in"<<endl;
    alive=false;
    mutex.unlock();
    std::cerr<<"Graphic:termiante out"<<endl;
}
