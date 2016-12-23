#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageLogger>
#include "qcustomplot.h"
#include "SerialPort.h"
#include "Communication.h"
#include "mytcpserver.h"
#include "graphicthread.h"
#include "sim3dthread.h"
#include "arduinothread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    Ui::MainWindow* getUI(){return ui;}
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent (QCloseEvent *event);

signals:
    void sim3DReq();
    void UpdatePidSignal(int,int,int);
private slots:
    void on_btnConnPlate_clicked();

    void updateServoPlotData(int,int);
    void updateXYPlotData(int,int);

    void on_btnDisconnect_clicked();
    void on_btnOpen3D_clicked();


    void on_btnUpPID_clicked();

private:

    void setXYPlot();
    void setServoPlot();
    Ui::MainWindow *ui;
    QTimer timerXYPlot;
    QTimer timerServoPlot;

    Sim3DThread *simThread;
   // GraphicThread *guiThread; // TODO: check this
    ArduinoThread *ardThread;

    Communication *com = NULL;

    bool isSim3DConnected = false;
    bool connectionCompleted =false;

    QMutex mtx;
    QProcess *proc3D;

    QMessageLogger logger;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
