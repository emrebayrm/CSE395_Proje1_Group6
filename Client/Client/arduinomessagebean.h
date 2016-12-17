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

};

#endif // ARDUINOMESSAGEBEAN_H
