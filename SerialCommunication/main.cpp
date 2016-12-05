#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Communucation.h"
#include <csignal>
using namespace std;
Communucation *com;
void signalHandler(int i){
    com->write('F');
    exit(0);
}

int main() {
    com = new Communucation("/dev/ttyACM0",SerialPort::BR_9600);
    int i;
    signal(SIGINT,signalHandler);
    while (1) {
        if(com->read()){
            cout << "motor1 angle " << com->getMotor1Angle() << endl;
            cout << "motor2 angle " << com->getMotor2Angle() << endl;
            cout << " Entry :: " << i++ << endl << endl;
        }else {
            cout << "Board has left";
        }
    }
    return 0;
}