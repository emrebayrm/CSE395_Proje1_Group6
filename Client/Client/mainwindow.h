#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageLogger>
#include "graphicthread.h"
#include "requirements.h"



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
public slots:
    void updateServoPlotData();
    void updateXYPlotData();

    void ardConnection();
    void sim3DConnection();

private slots:
    void on_btnConnPlate_clicked();

    void on_btnDisconnect_clicked();
    void on_btnOpen3D_clicked();

private:

    void setXYPlot();
    void setServoPlot();
    Ui::MainWindow *ui;
    QTimer timerXYPlot;
    QTimer timerServoPlot;

    Sim3DThread *simThread;
    GraphicThread *guiThread; // TODO: check this
    ArduinoThread *ardThread;

    Communication *com;
    myTcpServer *server;

    bool isSim3DConnected = false;
    bool connectionCompleted =false;

    QMutex mtx;
    QProcess *proc3D;

    QMessageLogger logger;
};

#endif // MAINWINDOW_H
