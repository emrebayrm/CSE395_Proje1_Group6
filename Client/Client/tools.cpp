#include"tools.h"

Tool::CommunucateWithArduino(void *message){
    threadMessage message = (threadMessage)message;

    Communucation com(message.portname,SerialPort::BR_9600);

    while (1) {
        if(com.readUntil()){
            //Todo:
        }
    }
}
