#include"tools.h"
/*
 * read 0
 * write 1
            */
using namespace std;
namespace Tool{
    char Packet_format[] = "{%lf %lf %d %d}";
    bool isButtonPressed; // uses Unreal Button pressed

    void* CommunicateWithArduino(void *message){
        Tool::threadMessageArdu *mes = (Tool::threadMessageArdu*)message;
        Tool::threadMessage3D message3D;
        Tool::threadMessageGrafik messageGrafik;

        int fdGrafik[2];
        pthread_t idGrafik;
        int fd3DSim[2];
        pthread_t id3DSim;

        pipe(fdGrafik); //create pipe between grafik draw.
        //SetMessage
        messageGrafik.pipefd[0] = fdGrafik[0];
        messageGrafik.pipefd[1] = fdGrafik[1];

        int error = pthread_create(&idGrafik,NULL,CommunicateWithGrafik,&messageGrafik);//create thread for Grafic Drawing.

        if(error){
            std::cerr << "Error Creating Grafic thread" << std::endl;
            //TODO: Exit
        }

        pipe(fd3DSim);     //create pipe for 3Dsim
        //Set Message
        message3D.pipefd[0] = fd3DSim[0];
        message3D.pipefd[1] = fd3DSim[1];

        error = pthread_create(&id3DSim,NULL,CommunicateWith3DSim,&message3D);          //create thread for 3Dsim

        if(error){
            std::cerr << "Error Creating 3DSim thread" << std::endl;
            //TODO: Exit
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

                sprintf(sendBuffer,Packet_format,com.getXMotorAngle(),
                                                 com.getYMotorAngle(),
                                                 com.getXMotorAngle(),
                                                 com.getYMotorAngle());

                writeRet = write(fdGrafik[1],sendBuffer,sizeof(char)*PACKET_SIZE);   //Send packet to grafik pipe.
                if(writeRet < 0){
                    //TODO: ERROR
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
                        //TODO: ERROR
                    }
                }

                //check 3D sim is quitted ?
                if(std::strcmp(getBuffer,QUIT) == 0){
                    //pthread_join(id3DSim,NULL);
                    sim3DisOpen = false;
                }
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
        while(1){
            if (isButtonPressed) {
                strcpy(buf,PRESSED);
                write(fds[1],buf,THREADCOMSIZE); //Send Message PRESSED

                mkfifo(FIFONAME,0666); //create fifo

                int fifofd = open(FIFONAME,O_RDWR);

                pid_t pid = fork(); // fork for 3d sim

                if(pid == 0){
                    execl(EXEADDRESS," "); //start exe

                    //exit(EXIT_SUCCESS);
                }

                sprintf(procname,"/proc/%d",(int)pid);

                write(fds[1],buf,THREADCOMSIZE * sizeof(char));   //then send Ready message to pipe

                do{
                    read(fds[0],buf,PACKET_SIZE* sizeof(char));   //then get packet from pipe
                    write(fifofd,buf,PACKET_SIZE * sizeof(char)); //send Packet to fifo
                }while(!(stat(procname, &sts) == -1 && errno == ENOENT));//until Exe died

                strcpy(buf,QUIT);
                write(fds[1],buf,5); //send Exe Died Message
            }
        }
    }

    void* CommunicateWithGrafik(void* message){
        //DataUpdater::getInstance()->updateXYPlotData(15,15);

        /*threadMessageGrafik mes = *((threadMessageGrafik*) message);
        int fds[2]; fds[0] = mes.pipefd[0]; fds[1] = mes.pipefd[1];
        char buffer[PACKET_SIZE];
        int ballX,ballY;
        float motorXAngle,motorYAngle;
        while(1){
            read(fds[0],buffer,sizeof(char)*PACKET_SIZE); //get Packet
            //Parse
            sscanf(buffer,Packet_format,&motorXAngle,&motorYAngle,&ballX,&ballY);

            // update X-Y coordinate graphic

        }*/
    }

}
