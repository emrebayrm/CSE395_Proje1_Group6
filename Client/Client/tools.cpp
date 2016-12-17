
//author : Emre BAYRAM

#include"tools.h"

using namespace std;

namespace Tool{

    bool isButtonPressed; // uses Unreal Button pressed

    void* CommunicateWithArduino(void *message){
        threadMessageArdu *mes = (Tool::threadMessageArdu*)message;
        threadMessage3D message3D;
        threadMessageGrafic messageGrafic;

        int fdGrafic[2];
        pthread_t idGrafic;
        int fd3DSim[2];
        pthread_t id3DSim;

        pipe(fdGrafic); //create pipe between grafik draw.
        //SetMessage
        messageGrafic.pipefd[0] = fdGrafic[0];
        messageGrafic.pipefd[1] = fdGrafic[1];

        int error = pthread_create(&idGrafic,NULL,CommunicateWithGrafic,&messageGrafic);//create thread for Grafic Drawing.

        if(error){
            std::cerr << "Error Creating Grafic thread" << std::endl;
            exit(EXIT_FAILURE);
        }

        pipe(fd3DSim);     //create pipe for 3Dsim
        //Set Message
        message3D.pipefd[0] = fd3DSim[0];
        message3D.pipefd[1] = fd3DSim[1];

        error = pthread_create(&id3DSim,NULL,CommunicateWith3DSim,&message3D);          //create thread for 3Dsim

        if(error){
            std::cerr << "Error Creating 3DSim thread" << std::endl;
            exit(EXIT_FAILURE);
        }

        QString str = mes->portname;
        //std::cout<<str.toStdString();
        Communication com(str.toStdString(),SerialPort::BR_9600);
        char sendBuffer[PACKET_SIZE];
        char getBuffer[THREADCOMSIZE];
        bool sim3DisOpen = false;
        int writeRet;
        while (1) {
            if(com.readUntil()){

            sprintf(sendBuffer,PACKETFORMAT,com.getXMotorAngle(),
                                             com.getYMotorAngle(),
                                             com.getBallXCoordinate(),
                                             com.getBallYCoordinate());

            std::cerr << com.getBallXCoordinate()  << std::endl;
            writeRet = write(fdGrafic[1],sendBuffer,sizeof(char)*PACKET_SIZE);   //Send packet to grafik pipe.
            if(writeRet < 0){
                std::cerr << "Error Writing " << std::endl;
                exit(EXIT_FAILURE);
            }
            read(fd3DSim[0],getBuffer,THREADCOMSIZE);

            //check button unreal button has pressed.
            if(std::strcmp(getBuffer,PRESSED) == 0){
                //TODO:Pressed Section
                sim3DisOpen = true;
            }

            //check 3D is open if openned check if it is ready Message
            if(sim3DisOpen && (std::strcmp(getBuffer,READY)) == 0){
                writeRet = write(fd3DSim[1],sendBuffer,sizeof(char)*PACKET_SIZE);//Send Packet to 3d

                if(writeRet < 0){
                    std::cerr << "Write Error. EXITING !!!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            //check 3D sim is quitted ?
            if(std::strcmp(getBuffer,QUIT) == 0){
                //pthread_join(id3DSim,NULL);
                sim3DisOpen = false;
            }
            //Send To Grafic Thread
            writeRet = write(fdGrafic[1],sendBuffer,sizeof(char)*PACKET_SIZE);
            }//EndRead
        }//End while
    }

    void* CommunicateWith3DSim(void* message){
        //Add Action listener button
        threadMessage3D mes = *((threadMessage3D*) message);
        char buf[PACKET_SIZE]; //Buffer
        char procname[30]; // Procces name
        struct stat sts; // for checking is there exist still

        int fds[2] ; fds[0] = mes.pipefd[0];fds[1] = mes.pipefd[1];
        strcpy(buf,NONE);
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(0x0202, &wsaData);
#endif

    }

    void *CommunicateWithGrafic(void* message){
        threadMessageGrafic mes = *((threadMessageGrafic*) message);
        int fds[2]; fds[0] = mes.pipefd[0]; fds[1] = mes.pipefd[1];
        char buffer[PACKET_SIZE];
        int ballX,ballY;
        double motorXAngle,motorYAngle;
        while(1){
            read(fds[0],buffer,sizeof(char)*PACKET_SIZE); //get Packet
            //Parse
            sscanf(buffer,PACKETFORMAT,&ballX,&ballY,&motorXAngle,&motorYAngle);

        }

    }
}
