#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setServoPlot();
    setXYPlot();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setXYPlot(){

    // add X Coordinat Graph
    ui->plotXY->addGraph();
    ui->plotXY->graph(0)->setPen(QPen(Qt::blue));
    ui->plotXY->graph(0)->setName("X Koordinatı");

    // add Y Coordinat graph
    ui->plotXY->addGraph();
    ui->plotXY->graph(1)->setPen(QPen(Qt::red));
    ui->plotXY->graph(1)->setName("Y Koordinatı");

    // add legend to show graph names
    ui->plotXY->legend->setVisible(true);
    ui->plotXY->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom|Qt::AlignRight);

    // add time -> x axis
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotXY->xAxis->setTicker(timeTicker);
    ui->plotXY->axisRect()->setupFullAxesBox();


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plotXY->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotXY->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotXY->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotXY->yAxis2, SLOT(setRange(QCPRange)));

    // set y axis
    ui->plotXY->yAxis->setLabelColor("blue");
    ui->plotXY->yAxis->setLabel("Koordinatlar");
    ui->plotXY->yAxis->setRange(0,400); // y axis -> coordinates

    // set x axis
    ui->plotXY->xAxis->setLabel("Zaman");
    ui->plotXY->xAxis->setLabelColor("blue");

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timerXYPlot,SIGNAL(timeout()),this,SLOT(updateXYPlotData()));
    timerXYPlot.start(0);

}


void MainWindow::updateXYPlotData(){

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    double coordX=150;
    double coordY=200;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plotXY->graph(0)->addData(key, coordX);
      ui->plotXY->graph(1)->addData(key, coordY);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotXY->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plotXY->replot();

      ui->textBPlotXY->setText(
            QString("X: %1 \t  Y: %2")
            .arg(coordX)
            .arg(coordY));

}


void MainWindow::setServoPlot(){

    // add X Coordinat Graph
    ui->plotServo->addGraph();
    ui->plotServo->graph(0)->setPen(QPen(Qt::blue));
    ui->plotServo->graph(0)->setName("SERVO X");

    // add Y Coordinat graph
    ui->plotServo->addGraph();
    ui->plotServo->graph(1)->setPen(QPen(Qt::red));
    ui->plotServo->graph(1)->setName("SERVO Y");

    // add legend to show graph names
    ui->plotServo->legend->setVisible(true);
    ui->plotServo->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom|Qt::AlignRight);

    // add time -> x axis
    QSharedPointer<QCPAxisTickerTime> timeTicker2(new QCPAxisTickerTime);
    timeTicker2->setTimeFormat("%h:%m:%s");
    ui->plotServo->xAxis->setTicker(timeTicker2);
    ui->plotServo->axisRect()->setupFullAxesBox();


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plotServo->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotServo->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotServo->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotServo->yAxis2, SLOT(setRange(QCPRange)));

    // set y axis
    ui->plotServo->yAxis->setLabelColor("blue");
    ui->plotServo->yAxis->setLabel("Servo Açıları(°)");
    ui->plotServo->yAxis->setRange(0,180); // y axis -> coordinates

    // set x axis
    ui->plotServo->xAxis->setLabel("Zaman");
    ui->plotServo->xAxis->setLabelColor("blue");

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timerServoPlot,SIGNAL(timeout()),this,SLOT(updateServoPlotData()));
    timerServoPlot.start(0);
}


void MainWindow::updateServoPlotData(){

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    double servoXAngle=124;
    double servoYAngle=42;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plotServo->graph(0)->addData(key, servoXAngle);
      ui->plotServo->graph(1)->addData(key, servoYAngle);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotServo->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plotServo->replot();

    ui->textBPlotServo->setText(
          QString("Servo X: %1° \t  ServoY: %2°")
          .arg(servoXAngle)
          .arg(servoYAngle));


}


