#include "mainwindow.h"
#include <QApplication>
#include "Communucation.h"
#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  
  return a.exec();
}


//void signalHandler(int i){
//    com->write('F');
//    exit(0);
//}

//int main() {
//    com = new Communucation("/dev/ttyACM0",SerialPort::BR_9600);
//    int i;
//    signal(SIGINT,signalHandler);
//    while (1) {
//        if(com->readUntil()){
//            cout << "motorx angle " << com->getXMotorAngle() << endl;
//            cout << "motory angle " << com->getYMotorAngle() << endl;
//            cout << "Ball X :" << com->getBallXCoordinate() << endl;
//            cout << "Ball Y :" << com->getBallYCoordinate() << endl;
//            cout << " Entry :: " << i++ << endl << endl;
//        }else {
//            cout << "Board has left";
//        }
//    }
//    return 0;
//}
