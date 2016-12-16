#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
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
    void testThread();

public slots:
    void updateServoPlotData();
    void updateXYPlotData();

private:
    void setXYPlot();
    void setServoPlot();
    Ui::MainWindow *ui;
    QTimer timerXYPlot;
    QTimer timerServoPlot;

    GraphicThread *guiThread;


};

#endif // MAINWINDOW_H
