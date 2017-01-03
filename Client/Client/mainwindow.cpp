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
    ui->gtuLogo->setPixmap(QPixmap::fromImage(logo.scaled(201,111)));

    ui->rBCenter->setChecked(true);
    ardThread = new ArduinoThread(this);
    ardThread->mtx = &mtx;
    simThread->mtx = &mtx;
    simThread = new Sim3DThread(this);
    ardThread->simThread = simThread;
    com=NULL;
    proc3D =NULL;

    scene = new QGraphicsScene(0,0,400,300);
    ui->graphicsView->setScene(scene);

    connect(ardThread,SIGNAL(updateXYPlotDataArd(int,int)),this,SLOT(updateXYPlotData(int,int)));
    connect(ardThread,SIGNAL(updateServoPlotDataArd(int,int)),this,SLOT(updateServoPlotData(int,int)));
    connect(simThread,SIGNAL(Request(int)),ardThread,SLOT(HandleRequest(int)));
    connect(this,SIGNAL(sim3DReq()),ardThread,SLOT(Started()));
    connect(ardThread,SIGNAL(readySend(int,int,int,int)),simThread,SLOT(SendData(int,int,int,int)));
    connect(this,SIGNAL(UpdateXPidSignal(int,int,int)),ardThread,SLOT(updateXPID(int,int,int)));
    connect(this,SIGNAL(UpdateYPidSignal(int,int,int)),ardThread,SLOT(updateYPID(int,int,int)));
    connect(this,SIGNAL(ModeSent(int)),ardThread,SLOT(ChangeMode(int)));
}

MainWindow::~MainWindow()
{
    delete scene;
    scene=NULL;
    delete ui;
    ui=NULL;
    qDebug("MainWindow destructed!");
}


// initialize XY graphic plot
void MainWindow::setXYPlot(){

    // add X Coordinat Graph
    ui->plotXY->addGraph();
    ui->plotXY->graph(0)->setPen(QPen(Qt::blue));
    ui->plotXY->graph(0)->setName("X Coordinate");

    // add Y Coordinat graph
    ui->plotXY->addGraph();
    ui->plotXY->graph(1)->setPen(QPen(Qt::red));
    ui->plotXY->graph(1)->setName("Y Coordinate");

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

}

void MainWindow::updateXYPlotData(int bx,int by){
    scene->clear();
    scene->addLine(200,0,200,300);
    scene->addLine(0,150,400,150);
    scene->addEllipse(bx-10,300-by-10,20,20,QPen(Qt::red),QBrush(Qt::blue));

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
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotXY->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->plotXY->replot();

    ui->textBPlotXY->setText(
                QString("X: %1 \t  Y: %2").arg(coordX,4)
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

        ui->textBMsg->append("Connection:");
        ui->textBMsg->append(" Port name:"+portName);
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

        if(com!=NULL){
            com->closeConnection();
            delete com;
            com=NULL;
        }
        connectionCompleted=false;
        isSim3DConnected=false;
        ui->textBMsg->append("Connection closed!");
        event->accept();
    }
}

void MainWindow::on_btnOpen3D_clicked()
{
    if(ardThread->isAlive()){
        if(!isSim3DConnected){
            ui->textBMsg->append("3D Simulation is opennig ... ");

            ardThread->simThread = this->simThread;
            simThread->start();
            emit sim3DReq();
        }else{
            ui->textBMsg->append("Simulation already runnig");
        }
    }
    else{
        ui->textBMsg->append("Pls Connect Arduino first");
    }
}

void MainWindow::on_btnDisconnect_clicked()
{
    qDebug("test");
    if(com!=NULL){
        com->closeConnection();
        delete com;
        com=NULL;
    }

    while(simThread->isRunning()){
        simThread->terminate();
    }

    while(ardThread->isRunning()){
        ardThread->terminate();
    }

    connectionCompleted=false;
    isSim3DConnected=false;
    ui->textBMsg->append("Connection closed!");
}

void MainWindow::on_xPIDUp_clicked()
{
    if(ardThread->isAlive()){
        emit UpdateXPidSignal(ui->xKpEdit->text().toInt(),
                              ui->xKiEdit->text().toInt(),
                              ui->xKpEdit->text().toInt());
    }
    else{
        ui->textBMsg->append("Connection Hasn't Opened yet");
    }
}

void MainWindow::on_yPIDUp_clicked()
{
    if(ardThread->isAlive()){
    emit UpdateYPidSignal(ui->yKpEdit->text().toInt(),
                          ui->yKiEdit->text().toInt(),
                          ui->yKpEdit->text().toInt());
    }
    else{
        ui->textBMsg->append("Connection Hasn't Opened yet");
    }
}

void MainWindow::on_btnPlayGame_clicked()
{
    if(ardThread->isAlive()){
        if(ui->rBCenter->isChecked()){
            emit ModeSent(1);
            ui->textBMsg->append("Ball is Going to Center ");
            ui->textBMsg->append("Be Patient !");
        }else if(ui->rBCircle->isChecked()){
            ui->textBMsg->append("Ball is Drawing Circle ");
            ui->textBMsg->append("Be Patient !");
            emit ModeSent(2);
        }else if(ui->rBSquare->isChecked()){
            ui->textBMsg->append("Ball is Drawing Square ");
            ui->textBMsg->append("Be Patient !");
            emit ModeSent(3);
        }else if(ui->rBLightGame->isChecked()){
            ui->textBMsg->append("");
            ui->textBMsg->append("Take The Ball ");
            ui->textBMsg->append("Now Put your finger to three 3 points ");
            emit ModeSent(4);
        }
        else{
            ui->textBMsg->append("Something Wrong about Radio Button");
        }
    }
    else{
        ui->textBMsg->append("Arduino Hasn't Connnected yet");
    }
}
