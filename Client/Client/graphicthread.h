#ifndef GRAPHICTHREAD_H
#define GRAPHICTHREAD_H

#include <QtCore>
#include <QThread>
#include "arduinomessagebean.h"

class GraphicThread : public QThread
{
    Q_OBJECT
public:
    explicit GraphicThread(QObject* parent=0);
    void run();
    bool isAlive();
    ArduinoMessageBean msg;
    void terminate();
signals:

    void startThread();
private:
    QMutex mutex;
    bool alive;
};

#endif // GRAPHICTHREAD_H
