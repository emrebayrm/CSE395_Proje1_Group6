#ifndef TOOL_H
#define TOOL_H
#include"Communucation.h"
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
        int pipeid;
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageGrafik {
        int pipeid;
    };

    void CommunucateWithArduino(void* message);
    void CommunucateWith3DSim(void* message);
    void CommunucateWithGrafik(void* message);
}
#endif // TOOL_H
