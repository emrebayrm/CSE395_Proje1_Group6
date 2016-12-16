#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "requirements.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateXYPlotData();
    void updateServoPlotData();
private:
    void setXYPlot();
    void setServoPlot();
    Ui::MainWindow *ui;
    QTimer timerXYPlot;
    QTimer timerServoPlot;
};

#endif // MAINWINDOW_H
