//
// Created by emre on 03.12.2016.
//

#ifndef PROJE_COMMUNUCATION_H
#define PROJE_COMMUNUCATION_H

#include "SerialPort.h"
#include <string>

#define START_CHAR '{'
#define END_CHAR '}'
#define PACKET_SIZE 25
#define X_MOTOR_START 'X'
#define Y_MOTOR_START 'Y'
#define BALL_X_COORDINATE 'x'
#define BALL_Y_COORDINATE 'y'
#define XCOORDMMAX 940
#define XCOORDMIN 140
#define YCOORDMMAX 960
#define YCOORDMIN 130
#define XOUTMAX 40
#define XOUTMIN -40
#define YOUTMAX 30
#define YOUTMIN -30

using namespace std;

/**
 * Serial Commucation
 */
class Communucation {
private :
    bool communicationReady ;
    double XMotorAngle ;
    double YMotorAngle;
    int ballXCoordinate;
    int ballYCoordinate;
    SerialPort *port;
    bool makeHandShake();
    void checkConnection();
    inline void setXMotorAngle(double angle){ XMotorAngle = angle };
    inline void setYMotorAngle(double angle){YMotorAngle = angle };
    inline void setBallXCoordinate(int x){ballXCoordinate = map(x,XCOORDMIN,XCOORDMMAX,XOUTMIN,XOUTMAX)};
    inline void setBallYCoordinate(int y){ballYCoordinate = map(y,YCOORDMIN,YCOORDMMAX,YOUTMIN,YOUTMAX)};
    inline float map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
    }
public:
    /**
     * Constructor
     * str : Serail port name
     * baud : baud rate
     */
    Communucation(string str,SerialPort::BaudRate baud);
    /**
     * Attention: received data should be like ("X%3.2lfY%3.2lfx%dy%d!",double ,double,int,int)
     * Reads the serial port and assigns the motor angles
     */
    //bool read();

    bool readUntil();

    bool isCommunicationReady() const {
        return communicationReady;
    }

    double getXMotorAngle() const {
        return XMotorAngle;
    }


    double getYMotorAngle() const {
        return YMotorAngle;
    }

    int getBallXCoordinate() const {
        return ballXCoordinate;
    }

    int getBallYCoordinate() const {
        return ballYCoordinate;
    }

    bool write(char *msg);

    bool write(char ch);

};


#endif //PROJE_COMMUNUCATİON_H