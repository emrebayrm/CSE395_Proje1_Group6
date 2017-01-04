#include "sim3dthread.h"
#include <ctime>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

#ifdef _WIN32
    #include <io.h>
    // link with Ws2_32.lib
    #pragma comment(lib,"Ws2_32.lib")

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <stdio.h>
    #include <stdlib.h>   // Needed for _wtoi
    #include <windows.h>
    #define DEFAULT_BUFLEN 512
    #define DEFAULT_PORT "9999"
    #ifndef UNICODE
        #define UNICODE 1
    #endif
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
#endif
Sim3DThread::Sim3DThread(QObject *parent):QThread(parent)
{
    std::cerr << "Sim 3d thread is creating " << std::endl;
    this->alive = false;
    connectionCompleted = false;
}

void Sim3DThread::run(){
   if(proc3D==NULL){
        QString file ="./Simulation/BallAndPlate.exe";
        proc3D = new QProcess(this);
        proc3D->start(file);
        qDebug(file.toStdString().c_str());
        qDebug("proc3D started");
    }
    alive=true;
    int iResult;
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        //return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        //return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();

    }

    // Accept a client socket
    ClientSocket  = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);
    connsock =(int*) ClientSocket;
    int select;
    while(isAlive()){
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
             printf("Bytes received: %d\n", iResult);
             if(recvbuf[0] == 'G'){
                 select = 1;
                 msg.readyData = -1;
                 emit Request(select);
                 //    terminate();
     //                server->close();
     //                delete server;
     //                server= NULL;
                 }
              else if(recvbuf[0] == 'E'){
                 select = 0;
                 emit Request(select);
                 connectionCompleted = false;
                 close(connfd);
                 terminate();
     //            server->close();
     //            delete server;
     //            server= NULL;
     //            terminate();
             }
             else{
                 qDebug("Wrong ");
             }
        }

    //    std::string str = server->readData();

//        if(str.size() == 0)
//            continue;

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
    proc3D = NULL;
    //    qDebug("Sim3DThread::termiante out");
}

void Sim3DThread::SendData(int bx,int by,int mx, int my)
{
    char buffer[30];
    memset(buffer,0,30);
    std::sprintf(buffer,"C{ %d %d %d %d }",bx,by,mx,my);
    if(send((SOCKET) connsock, buffer, 30, 0 ))
        std::cerr << "Succesfully sent" << std::endl;

}
