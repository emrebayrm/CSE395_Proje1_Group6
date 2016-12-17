#ifndef GRAPHICTHREAD_H
#define GRAPHICTHREAD_H

#include <QtCore>
#include <QThread>
#include "arduinomessagebean.h"

class GraphicThread : public QThread
{
    Q_OBJECT
public:
    explicit GraphicThread(QObject* parent=0,bool b=false);
    void run();
    bool stop;
    ArduinoMessageBean msg;

signals:
    void startThread();
public slots:
};

#endif // GRAPHICTHREAD_H
