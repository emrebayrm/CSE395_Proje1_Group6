#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QCloseEvent>
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

private slots:
    void on_btnConnPlate_clicked();

    void on_btnDisconnect_clicked();

private:

    void setXYPlot();
    void setServoPlot();
    Ui::MainWindow *ui;
    QTimer timerXYPlot;
    QTimer timerServoPlot;

    GraphicThread *guiThread; // TODO: check this
    ArduinoThread *ardThread;
    pthread_t thArd;
    Communication *com;
    bool connectionCompleted =false;

    QMutex mtx;

};

#endif // MAINWINDOW_H
