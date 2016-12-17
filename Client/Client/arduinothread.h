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
    explicit ArduinoThread(QObject* parent=0,bool b=false);
    void run();
    bool stop;
    ArduinoMessageBean msg;
signals:
    void startArdThread();
public slots:

private:

};
#endif // ARDUINOTHREAD_H
