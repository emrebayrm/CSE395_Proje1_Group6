#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#define DEFAULTPORT 9999
#include <QObject>
#include "requirements.h"
class myTcpServer : public QObject
{
    Q_OBJECT
public:

    explicit myTcpServer(QObject *parent = 0);
    bool isEstablished(){return isEstablishedConnection;}
    bool SendData(char * Data);
    void listen();
    std::string readData();
    QString getAddress(){return server->serverAddress().toString();}

    int getPortNumber(){return server->serverPort();}

    void close(){socket->close();}
public slots:
    void newConnection();

private:
    QTcpServer *server;
    QTcpSocket *socket;

    bool isEstablishedConnection = false;
};

#endif // MYTCPSERVER_H
