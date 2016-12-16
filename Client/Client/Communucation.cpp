//
// Created by emre on 03.12.2016.
//

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <zconf.h>
#include "Communucation.h"

using namespace std;

Communucation::Communucation(string str, SerialPort::BaudRate baud) {
    port = new SerialPort(str,baud);
    communicationReady = false;
    checkConnection();
    makeHandShake();
    XMotorAngle = 999;
    YMotorAngle = 999;
    ballXCoordinate = 999;
    ballYCoordinate = 999;
}
//bool Communucation::read() {
//    if(!communicationReady)
//        return false;
//    write('N');
//    char ch;
//    int i=0;
//    port->wait(50);
//    do {
//        port->read(&ch);
//        if(ch == 'F')
//            return false;
//    }while(ch != 'X');
//    char inp[8];
//    // Get x motor angle
//    do{
//        port->read(&ch);
//        if(ch == 'Y')
//            break;
//        else
//            inp[i++] = ch;
//    }while(i < 7);
//    inp[i] = '\0';
//
//    XMotorAngle = atof(inp);
//    // Get y motor angle
//    i=0;
//    do{
//        port->read(&ch);
//        if(ch == 'x')
//            break;
//        else
//            inp[i++] = ch;
//    }while(i < 7);
//    inp[i] = '\0';
//    YMotorAngle = atof(inp);
//    // Get ball x coordinate
//    i=0;
//    do{
//        port->read(&ch);
//        if(ch == 'y')
//            break;
//        else
//            inp[i++] = ch;
//    }while(i < 7);
//    inp[i] = '\0';
//    ballXCoordinate = atoi(inp);
//    ballXCoordinate = map(ballXCoordinate,125,965,-30,30);
//    // Get ball y coordinate
//    i=0;
//    do{
//        port->read(&ch);
//        if(ch == '!')
//            break;
//        else
//            inp[i++] = ch;
//    }while(i < 7);
//    inp[i] = '\0';
//    ballYCoordinate = atoi(inp);
//    ballYCoordinate = map(ballYCoordinate,130,910,-40,40);
//    // Read Finished
//    return true;
//}

bool Communucation::makeHandShake() {
    char ch;
    cerr << "Handshake starting" << endl;
    do {
        write('S');
        port->wait(100);
        port->read(&ch);
    }while(ch != 'R');
    cerr << "Handshake succesfull" << endl;
    communicationReady = true;
    return true;
}

bool Communucation::write(char *msg) {
    return port->write(msg);
}

bool Communucation::write(char ch) {
    return port->write(ch);
}

void Communucation::checkConnection() {
    while( !( port->open()) ){
        usleep(100);
        cerr << "Open failed" << endl;
        cerr << "Trying to connect again " << endl;
    }
    while( !( port->prepare()) ){
        usleep(100);
        cerr << "Prepare failed" << endl;
        cerr << "Trying to connect again " << endl;
    }
}

bool Communucation::readUntil() {
    if(!communicationReady)
        return false;
    write('N');
    double xangle,yangle;
    int x,y;
    string input;

    port->readUntil(input,END_CHAR);
    //{XdoubleYdoublexintyint}
    sscanf(input.c_str(),"{X%lfY%lfx%dy%d}",&xangle,&yangle,&x,&y);
    //if is not valid values false 
    setXMotorAngle(xangle);
    setYMotorAngle(yangle);
    setBallXCoordinate(x);
    setBallYCoordinate(y);

    return true;
}











