#ifndef ARDUINOMESSAGEBEAN_H
#define ARDUINOMESSAGEBEAN_H

#include <string>
#include "SerialPort.h"
#include "arduinomessagebean.h"

using std::string;

class ArduinoMessageBean
{
public:
    ArduinoMessageBean();
    string portName;
    SerialPort::BaudRate baudRate;
    int ballX;
    int ballY;
    float motorXangle;
    float motorYangle;

};

#endif // ARDUINOMESSAGEBEAN_H
