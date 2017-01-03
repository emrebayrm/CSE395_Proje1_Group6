#include "sim3dthread.h"
#include <ctime>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

Sim3DThread::Sim3DThread(QObject *parent):QThread(parent)
{
    this->alive = false;
    connectionCompleted = false;
}

void Sim3DThread::run(){
    alive=true;
    //Run Exec part
/*    if(proc3D==NULL){
        QString file ="/home/hasan/workspace/CSE395_Proje1_Group6/Client/Client/test";
        proc3D = new QProcess(this);
        proc3D->start(file);
        qDebug(file.toStdString().c_str());
        qDebug("proc3D started");
    }
    */
/*
        while(!server->isEstablished());*/
    int listenfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(DEFAULTPORT);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    std::cerr << listenfd << std::endl;
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

    int select;
    while(isAlive()){
        char recvBuff[5];
        read(connfd, recvBuff, 5);
    //    std::string str = server->readData();

//        if(str.size() == 0)
//            continue;
        if(recvBuff[0] == 'G'){
            select = 1;
            msg.readyData = -1;
            emit Request(select);
            //    terminate();
//                server->close();
//                delete server;
//                server= NULL;
            }
         else if(recvBuff[0] == 'E'){
            select = 0;
            emit Request(select);
            connectionCompleted = false;
            close(connfd);
            terminate();
            qDebug("Connection Lost");
//            server->close();
//            delete server;
//            server= NULL;
//            terminate();
        }
        else{
            qDebug("Wrong ");
        }
    }
   // qDebug("Sim3DThread closed");
}

bool Sim3DThread::isAlive(){
    bool res=false;
//      qDebug("Sim3DThread::isAlive in");
    res=alive;
//      qDebug("Sim3DThread::isAlive out");
    return res;
}

void Sim3DThread::terminate(){
//    qDebug("Sim3DThread::termiante in");
    alive=false;
    //    qDebug("Sim3DThread::termiante out");
}

void Sim3DThread::SendData(int bx,int by,int mx, int my)
{
    char buffer[30];
    std::sprintf(buffer,"C{ %d %d %d %d }",bx,by,mx,my);
    if(write(connfd,buffer,30))
        std::cerr << "Succesfully sent" << std::endl;

}
