#include <QtCore>
#include <ctime>
#include <iostream>
#include <cstdio>

#include "arduinothread.h"
#include "mainwindow.h"

ArduinoThread::ArduinoThread(QObject *parent):QThread(parent)
{

    this->alive=false;
    connectionCompleted = false;

}

void ArduinoThread::run(){
    std::clock_t start;
    double duration=0;
    start = std::clock();
    int bx,by,mx,my;
    alive=true;

    com = new Communication(msg.portName,msg.baudRate);
    if(!com->isCommunicationReady()){
        delete com;
        alive = false;
    }

    while(isAlive()){
//        mtx->lock();
        if(com->readUntil()){
            bx = com->getBallXCoordinate();
            by = com->getBallYCoordinate();
            mx = com->getXMotorAngle();
            my = com->getYMotorAngle();
            /*if(mode == 4){
                std::cerr << "Yem1x :" << bx << " Yem1y : " << by << std::endl;
                std::cerr << "Yem2x :" << mx << " Yem2y : " << my << std::endl;
            }*/
            msg.ballX = bx;
            msg.ballY = by;
            msg.motorXangle = mx;
            msg.motorYangle = my;
/*
            QString log;
            log.append("Bx:").append(QString::number(guiThread->msg.ballX)).append("  ");
            log.append("By:").append(QString::number(guiThread->msg.ballY)).append("  ");
            log.append("Sx:").append(QString::number(guiThread->msg.motorXangle)).append("  ");
            log.append("Sy:").append(QString::number(guiThread->msg.motorYangle)).append("  ");
            logger.debug(log.toStdString().c_str());
            logger.debug("Arduino <--> ardThread okeyy");*/
        }
        duration = (std::clock() - start) /(double) CLOCKS_PER_SEC;
//        std::cerr << duration << std::endl;
        if(duration > 0.05){
            start = std::clock();
            emit updateServoPlotDataArd(mx,my);
            emit updateXYPlotDataArd(bx,by);
        }
//        mtx->unlock();
    }
    std::cerr<<"ArduinoThread closed"<<endl;
}

bool ArduinoThread::isAlive(){
    bool res=false;
//    mtx->lock();
//    std::cerr<<"Arduino::isAlive in"<<endl;
    res=alive;
//    mtx->unlock();
//    std::cerr<<"Arduino::isAlive out"<<endl;
    return res;
}

void ArduinoThread::terminate(){
    mtx->lock();
    std::cerr<<"Arduino::termiante in"<<endl;
    alive=false;
    mtx->unlock();
    std::cerr<<"Arduino::termiante out"<<endl;
}

void ArduinoThread::updateXPID(int kp, int ki, int kd)
{
    mtx->lock();
    char Buffer[15];
    sprintf(Buffer,"X %d %d %d",kp,ki,kd);
    std::cout << "Buffer Writing : " << Buffer << std::endl;
    com->write(Buffer);
    std::cout << "Succefuly Updated" << std::endl;
    mtx->unlock();
}

void ArduinoThread::updateYPID(int kp, int ki, int kd)
{
    mtx->lock();
    char Buffer[15];
    sprintf(Buffer,"Y %d %d %d",kp,ki,kd);
    std::cout << "Buffer Writing : " << Buffer << std::endl;
    com->write(Buffer);
    std::cout << "Succefuly Updated" << std::endl;
    mtx->unlock();
}

void ArduinoThread::Started()
{
    sim3dStarted = true;
}

void ArduinoThread::HandleRequest(int mode){
    if(mode == 0){
        simThread->terminate();
    }
    else if(mode == 1 && sim3dStarted)
    {
        emit readySend(msg.ballX,msg.ballY,msg.motorXangle,msg.motorYangle);
    }

}
void ArduinoThread::ChangeMode(int mode){
    char buffer[5];
    switch (mode) {
    case 1:
        sprintf(buffer,"M C");
        mode = 1;
        break;
    case 2:
        sprintf(buffer,"M O");
        mode = 2;
        break;
    case 3:
        sprintf(buffer,"M S");
        mode = 3;
        break;
    case 4:
        sprintf(buffer,"M L");
        mode = 4;
        break;
    default:
        break;
    }
    std::cerr << "Mode : " << buffer << std::endl;
    com->write(buffer);
}
void takeCoinInfo(int* first,int* second,int* third){

}
