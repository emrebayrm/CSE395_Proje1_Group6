//
// Created by emre on 03.12.2016.
//

#include <iostream>

#include <stdlib.h>
#include <zconf.h>
#include "Communucation.h"

Communucation::Communucation(string str, SerialPort::BaudRate baud) {
    port = new SerialPort(str,baud);
    readytocom = false;
    if(!port->open()){
        std::cout << "Port couldn't Opened \n";
    }

    if(!port->prepare()){
        std::cerr << "Not prepared \n";
    }

    makeHandShake();
    angleMotor1 = -1;
    angleMotor2 = -1;
}

bool Communucation::read() {
    if(!readytocom)
        return false;
    char ch;
    int i=0;
    port->wait(50);
    do {
        //std::cerr<< ch;
        port->read(&ch);
        if(ch == 'F')
            return false;
    }while(ch != 'X');
    char inp[7];

    do{

        port->read(&ch);
        if(ch == 'Y')
            break;
        else
            inp[i++] = ch;
    }while(i < 7);
    inp[i] = '\0';
    angleMotor1 = atof(inp);
    i=0;
    do{
        port->read(&ch);
        if(ch == '!')
            break;
        else
            inp[i++] = ch;
    }while(i < 7);
    inp[i] = '\0';
    angleMotor2 = atof(inp);

    // Read Finished
    write('N');
    return true;
}

bool Communucation::makeHandShake() {
    write('S');
    char ch;
    do {
        port->read(&ch);
    }while(ch != 'R');
    readytocom = true;
    return true;
}

bool Communucation::write(char *msg) {
    return port->write(msg);
}

bool Communucation::write(char ch) {
    return port->write(ch);
}







