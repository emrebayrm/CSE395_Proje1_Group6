#include"tools.h"

void Tool::CommunucateWithArduino(void *message){
    threadMessage message = (threadMessage)message;
    //create pipe between grafik çizme.
    //create thread for Grafik Çizme.
    //create pipe for 3Dsim
    //create thread for 3Dsim
    Communucation com(message.portname,SerialPort::BR_9600);
    char Buffer[PACKET_SIZE];
    bool sim3DisOpen = false;
    while (1) {
        if(com.readUntil()){
            sprintf(Buffer,"{%lf %lf %d %d}",com.getXMotorAngle(),
                                             com.getYMotorAngle(),
                                             com.getXMotorAngle(),
                                             com.getYMotorAngle());
            //Send packet to grafik pipe.
            //check button unreal button has pressed.
            if(sim3DisOpen){
                //get 'ready' Message from 3d
                    //then
                    //Send Packet to 3d
            }

        }
    }
}

void Tool::CommunucateWith3DSim(void* message){
    //Add Action listener button
    threadMessage3D mes = (threadMessage3D) message;
    int fd = mes.pipeid;
    //open pipe
    //if button pressed
    //create fifo
    //start exe
        //get ready message from 3Dsim
        //then send Ready message to pipe
        //then get packet from pipe
        //send Packet to fifo
        //until Exe died
    //send Exe Died Message
}

void Tool::CommunucateWithGrafik(void* message){
    threadMessageGrafik mes = (threadMessageGrafik) message;
    int fd = mes.pipeid;
    //open pipe
    //get Packet
    //Draw Grafik
}
