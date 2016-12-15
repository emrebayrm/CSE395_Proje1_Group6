#ifndef TOOL_H
#define TOOL_H
#include"Communication.h"
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
              |     fifo
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
        int pipeid;
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageGrafik {
        int pipeid;
    };

    void CommunicateWithArduino(void* message);
    void CommunicateWith3DSim(void* message);
    void CommunicateWithGrafik(void* message);
}
#endif // TOOL_H
