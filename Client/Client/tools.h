#ifndef TOOL_H
#define TOOL_H
#include <fcntl.h>
#include<iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "Communication.h"

#define THREADCOMSIZE 6
#define PRESSED "pres"
#define READY "ready"
#define QUIT "quit"
#define FIFONAME "fifo"
#define EXEADDRESS "3dsim.exe"
char Packet_format[] = "{%lf %lf %d %d}";

static bool isButtonPressed; // uses Unreal Button pressed
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
             \ /    fifo
            Thread -----> Unreal Exe
                            */
namespace Tool {

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageArdu{
        char* portname;
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessage3D{
        int pipefd[2];
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageGrafik {
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
    void* CommunicateWithGrafik(void* message);
}
#endif // TOOL_H
