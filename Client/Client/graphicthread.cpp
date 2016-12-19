#include "graphicthread.h"
#include "mainwindow.h"
#include <QtCore>

GraphicThread::GraphicThread(QObject *parent,bool b):QThread(parent),stop(b)
{

}

void GraphicThread::run(){

    stop=false;
    while(!stop){
        // emit the signal for the count label
        emit startThread();

        // slowdown the count change, msec
        this->msleep(80);
    }

}
