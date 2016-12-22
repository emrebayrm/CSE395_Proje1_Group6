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

    QImage logo(":/images/gtuLogo500.png");
    ui->gtuLogo->setPixmap(QPixmap::fromImage(logo.scaled(400,200)));

    ui->rBCenter->setChecked(true);
    //guiThread = new GraphicThread(this);
    ardThread = new ArduinoThread(this);
    ardThread->mtx = &mtx;
    simThread->mtx = &mtx;
    simThread = new Sim3DThread(this);
    ardThread->simThread = simThread;
    com=NULL;
    server=NULL;
    proc3D =NULL;

    // guiTHreadi icinde startThread calistirilinca, bu clasın verilen metodunu calistir
/*    connect(guiThread,SIGNAL(startThread()),this,SLOT(updateServoPlotData()));
    connect(guiThread,SIGNAL(startThread()),this,SLOT(updateXYPlotData()));
*/
    connect(ardThread,SIGNAL(updateXYPlotDataArd(int,int)),this,SLOT(updateXYPlotData(int,int)));
    connect(ardThread,SIGNAL(updateServoPlotDataArd(int,int)),this,SLOT(updateServoPlotData(int,int)));
    connect(simThread,SIGNAL(Request(int)),ardThread,SLOT(HandleRequest(int)));
    connect(this,SIGNAL(sim3DReq()),ardThread,SLOT(Started()));
    connect(ardThread,SIGNAL(readySend(int,int,int,int)),simThread,SLOT(SendData(int,int,int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


// initialize XY graphic plot
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
    ui->plotXY->yAxis->setLabel("Coordinates");
    ui->plotXY->yAxis->setRange(0,400); // y axis -> coordinates

    // set x axis
    ui->plotXY->xAxis->setLabel("Time");
    ui->plotXY->xAxis->setLabelColor("blue");

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    //connect(&timerXYPlot,SIGNAL(timeout()),this,SLOT(updateXYPlotData()));
    //timerXYPlot.start(0);

}

void MainWindow::updateXYPlotData(int bx,int by){
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    int coordX=bx;
    int coordY=by;
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
            .arg(coordX,4)
            .arg(coordY,4));

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
    ui->plotServo->yAxis->setLabel("Angles(°)");
    ui->plotServo->yAxis->setRange(0,180); // y axis -> coordinates

    // set x axis
    ui->plotServo->xAxis->setLabel("Time");
    ui->plotServo->xAxis->setLabelColor("blue");

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
   // connect(&timerServoPlot,SIGNAL(timeout()),this,SLOT(updateServoPlotData()));
   // timerServoPlot.start(0);
}


void MainWindow::updateServoPlotData(int sx,int sy){

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    int servoXAngle=sx;
    int servoYAngle=sy;
  //  std::cerr << servoYAngle << std::endl;
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
          .arg(servoXAngle,4)
          .arg(servoYAngle,4));

}


void MainWindow::on_btnConnPlate_clicked()
{
    if(!ardThread->isAlive()){
        QString portName = ui->inputPortName->text();

        ArduinoMessageBean msg;
        msg.portName = portName.toStdString();
        msg.baudRate=SerialPort::BR_9600;

        ui->textBMsg->append("New connection:");
        ui->textBMsg->append(" Port name:"+portName);
        ui->textBMsg->append(" Baud rate:"+QString::number(msg.baudRate));
        ardThread->msg=msg;
        ardThread->start();
    }
    else
        ui->textBMsg-> append("Connection is already open.");
}


void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "CSE395 Client",
                                                                tr("Are you sure?\n"),QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        while(simThread->isRunning()){
            simThread->terminate();
        }

        while(ardThread->isRunning()){
            ardThread->terminate();
        }

//        while(guiThread->isRunning()){
//            guiThread->terminate();
//        }

        if(server != NULL ){
            server->close();
            delete server;
            server = NULL;
        }

        if(com!=NULL){
            com->closeConnection();
            delete com;
            com=NULL;
        }
        event->accept();
    }
}

void MainWindow::on_btnOpen3D_clicked()
{
    if(!isSim3DConnected){
        ui->textBMsg->append("3D Simulation is opennig ... ");
    /*    server = new myTcpServer(this);
        server->listen();
*/
  //      simThread->server = this->server;
        ardThread->simThread = this->simThread;
        simThread->start();
        emit sim3DReq();
    }else{
        ui->textBMsg->append("Simulation already runnig");
    }
}

void MainWindow::on_btnDisconnect_clicked()
{
    qDebug("test");

    while(simThread->isRunning()){
        simThread->terminate();
    }

    while(ardThread->isRunning()){
        ardThread->terminate();
    }

//    while(guiThread->isRunning()){
//        guiThread->terminate();
//    }

    if(server != NULL ){
        server->close();
        delete server;
        server = NULL;
    }

    if(com!=NULL){
        com->closeConnection();
        delete com;
        com=NULL;
    }

    connectionCompleted=false;
    isSim3DConnected=false;
    ui->textBMsg->append("Connection closed!");
}
