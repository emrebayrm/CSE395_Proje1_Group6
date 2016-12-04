//
// Created by emre on 03.12.2016.
//

#include <iostream>
#include <stdlib.h>
#include "Communucation.h"

Communucation::Communucation(string str, SerialPort::BaudRate baud) {
    port = new SerialPort(str,baud);
    angleMotor1 = -1;
    angleMotor2 = -1;
}

void Communucation::read() {
    string readdata;
    char angle[20];
    if(!port->open()){
        std::cout << "Port Acılamadı";
        return;
    }
    char ch;
    do {
        port->read(&ch);

    }while(ch != 'X');
    char inp[7];
    int i=0;
    do{

        port->read(&ch);
        if(ch == 'Y')
            break;
        else
            inp[i++] = ch;
    }while(1);
    inp[i] = '\0';
    angleMotor1 = atof(inp);
    i=0;
    do{
        port->read(&ch);
        if(ch == '!')
            break;
        else
            inp[i++] = ch;
    }while(1);
    inp[i] = '\0';
    angleMotor2 = atof(inp);

    port->close();
}


