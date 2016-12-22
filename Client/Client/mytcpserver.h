#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#define DEFAULTPORT 9999
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class myTcpServer : public QObject
{
    Q_OBJECT
public:
    QTcpServer *server = NULL;
    QTcpSocket *socket = NULL;

    explicit myTcpServer(QObject *parent = 0);
    bool isEstablished(){return isEstablishedConnection;}
    bool SendData(char * Data);
    void listen();
    std::string readData();
    QString getAddress(){return server->serverAddress().toString();}

    int getPortNumber(){return server->serverPort();}

    void close();
public slots:
    void newConnection();
private:
    bool isEstablishedConnection = false;
};

#endif // MYTCPSERVER_H
