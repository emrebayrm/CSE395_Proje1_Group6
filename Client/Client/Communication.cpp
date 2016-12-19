//
// Created by emre on 03.12.2016.
//

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <zconf.h>
#include "Communication.h"


using namespace std;

Communication::Communication(string str, SerialPort::BaudRate baud,QMutex *mtx) {
    cerr<<"new connection opened"<<endl;
    port = new SerialPort(str,baud);

    communicationReady = false;
    if(checkConnection())
        makeHandShake();
    XMotorAngle = 999;
    YMotorAngle = 999;
    ballXCoordinate = 999;
    ballYCoordinate = 999;
    this->mtx = mtx;
}

bool Communication::makeHandShake() {
    char ch;
    cerr << "Handshake starting" << endl;
    do {
        write('S');
        port->wait(100);
        port->read(&ch);
        cerr<<"CH:"<<ch<<endl;
    }while(ch != 'R');
    cerr << "Handshake succesfull" << endl;
    communicationReady = true;
    return communicationReady;
}

bool Communication::write(char *msg) {
    return port->write(msg);
}

bool Communication::write(char ch) {
    return port->write(ch);
}

bool Communication::checkConnection() {

    if(!port->open()){
        cerr << "Open failed" << endl;
        cerr << "Trying to connect again " << endl;
        return false;
    }
    if( !( port->prepare()) ){
        usleep(100);
        cerr << "Prepare failed" << endl;
        cerr << "Trying to connect again " << endl;
        return false;
    }
    return true;
}

void Communication::closeConnection()
{
    if(port!=NULL){
        port->write('F');
        port->close();
        delete port;
        port=NULL;
    }
    communicationReady=false;
}

bool Communication::readUntil() {
    int xangle,yangle;
    int x,y;
    string input;

    if(!communicationReady)
        return false;

    write('N'); // message to take coordinates

    port->readUntil(input,END_CHAR);
    cerr<<"SerialRead:"<<input<<endl;

    char temp;

    //{XintYintxintyint}
    sscanf(input.c_str(),"%c%c%d%c%d%c%d%c%d%c",&temp,&temp,&xangle,&temp,&yangle,&temp,&x,&temp,&y,&temp);
    //if is not valid values false 

    mtx->lock();
    cerr<<"inmutexCoom"<<endl;
    //cerr<<xangle<<" "<<yangle<<" "<<x<<" "<<y<<endl;
    setXMotorAngle(xangle);
    setYMotorAngle(yangle);
    setBallXCoordinate(x);
    setBallYCoordinate(y);
    cerr<<"outmutexcomm"<<endl;
    mtx->unlock();

    return true;
}
