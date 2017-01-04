
//author : Emre BAYRAM

#ifndef TOOL_H
#define TOOL_H

#include <fcntl.h>
#include<iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <cstring>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "Communication.h"
#include "requirements.h"

#define PORTNAMESIZE 30
#define THREADCOMSIZE 6
#define NONE "none"
#define PRESSED "pres"
#define READY "ready"
#define QUIT "quit"
#define DEFAULTPORT 53333
#define EXEADDRESS "3dsim.exe"
#define PACKETFORMAT "{ %3.2lf %3.2lf %3d %3d}"

/*
            Thread (grafik çizme)
              ^
        pipe  |
              |
              |    Serial
    Host-> Thread -----> Arduiono
              |
              |pipe
              |
             \ /    Socket
            Thread -------> Unreal Exe
                            */

namespace Tool {

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageArdu{
        char portname[PORTNAMESIZE];
        SerialPort::BaudRate baudrate;
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessage3D{
        int pipefd[2];
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageGrafic {
        int pipefd[2];
    };

    struct InfoPacket{
        int ballX;
        int ballY;
        float motorXangle;
        float motorYangle;
    };

    void* CommunicateWithArduino(void* message);
    void* CommunicateWith3DSim(void* message);
    void* CommunicateWithGrafic(void* message);
}
#endif // TOOL_H
