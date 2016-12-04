//
// Created by emre on 03.12.2016.
//

#ifndef PROJE_COMMUNUCATION_H
#define PROJE_COMMUNUCATION_H

#include "SerialPort.h"
#include <string>
using namespace std;

/**
 * Serial Commucation
 */
class Communucation {
    private :
        double angleMotor1 ;
        double angleMotor2;
        SerialPort *port;
    public:
        /**
         * Constructor
         * str : Serail port name
         * baud : baud rate
         */
        Communucation(string str,SerialPort::BaudRate baud);
        /**
         * Attention: received data should be like ("X%3.2lfY%3.2lf!",double ,double)
         * Reads the serial port and assigns the motor angles
         */
        void read();
        /**
         * Attention: you need call first read method
         * get motor1 Angle
         */
        inline double getMotor1Angle(){
            return angleMotor1;
        }
        /**
         * Attention: you need call first read method
         * get motor2 Angle
         */
        inline double getMotor2Angle(){
            return angleMotor2;
        }

};


#endif //PROJE_COMMUNUCATİON_H
