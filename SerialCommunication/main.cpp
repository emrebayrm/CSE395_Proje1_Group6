#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Communucation.h"

using namespace std;

int main() {
    Communucation com("/dev/ttyACM0",SerialPort::BR_9600);
    int i;
    while (com.read()) {
        cout << "motor1 angle " << com.getMotor1Angle() << endl;
        cout << "motor2 angle " << com.getMotor2Angle() << endl;
        cout << " Entry :: " << i++ << endl <<endl;

    }
    return 0;
}