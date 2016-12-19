#ifndef ARDUINOTHREAD_H
#define ARDUINOTHREAD_H

#include <QtCore>
#include <QThread>
#include "requirements.h"
//#include "tools.h"
#include "arduinomessagebean.h"


class ArduinoThread : public QThread
{
    Q_OBJECT
public:
    explicit ArduinoThread(QObject* parent=0);
    void run();
    bool isAlive();
    void terminate();
    ArduinoMessageBean msg;
signals:
    void startArdThread();
public slots:

private:
    QMutex mutex;
    bool alive;

};
#endif // ARDUINOTHREAD_H
