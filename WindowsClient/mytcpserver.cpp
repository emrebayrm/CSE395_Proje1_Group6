#include "mytcpserver.h"
#include <iostream>

myTcpServer::myTcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));
}

void myTcpServer::listen(){
    if(!server->listen(QHostAddress::Any,DEFAULTPORT ))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

std::string myTcpServer::readData()
{
    QByteArray data = socket->readAll();

    return data.toStdString();
}

void myTcpServer::close()
{
    if(server == NULL) {
        server->close();
        delete server;
    }
    if(socket == NULL){
        socket->close();
        delete socket;
    }
}



bool myTcpServer::SendData(char * data)
{
    return socket->write(data) > 0 ? true:false;
}

void myTcpServer::newConnection()
{
    // need to grab the socket
    std::cerr << "new conn " << std::endl;
    socket = server->nextPendingConnection();
    isEstablishedConnection = true;
}
