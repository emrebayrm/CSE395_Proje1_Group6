#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>

class myTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit myTcpServer(QObject *parent = 0);

signals:

public slots:
};

#endif // MYTCPSERVER_H