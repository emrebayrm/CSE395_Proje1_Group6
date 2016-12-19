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

    guiThread = new GraphicThread(this);
    ardThread = new ArduinoThread(this);
    simThread = new Sim3DThread(this);

    // guiTHreadi icinde startThread calistirilinca, bu clasın verilen metodunu calistir
    connect(guiThread,SIGNAL(startThread()),this,SLOT(updateServoPlotData()));
    connect(guiThread,SIGNAL(startThread()),this,SLOT(updateXYPlotData()));

    connect(ardThread,SIGNAL(startArdThread()),this, SLOT(ardConnection()));
    connect(simThread,SIGNAL(startThread()),this,SLOT(sim3DConnection()));
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
    ui->plotXY->yAxis->setLabel("Koordinatlar");
    ui->plotXY->yAxis->setRange(0,400); // y axis -> coordinates

    // set x axis
    ui->plotXY->xAxis->setLabel("Zaman");
    ui->plotXY->xAxis->setLabelColor("blue");

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    //connect(&timerXYPlot,SIGNAL(timeout()),this,SLOT(updateXYPlotData()));
    //timerXYPlot.start(0);

}


void MainWindow::updateXYPlotData(){
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    int coordX=guiThread->msg.ballX;
    int coordY=guiThread->msg.ballY;
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

void MainWindow::ardConnection()
{
    if(!connectionCompleted){
        com = new Communication(ardThread->msg.portName,ardThread->msg.baudRate,&mtx);
        if(com->isCommunicationReady()){
            if(!guiThread->isAlive())
                guiThread->start();
        }
    }
    if(!com->isCommunicationReady()){
        ardThread->terminate();
        ui->textBMsg->append("Arduino Connection failed");
        logger.warning("Arduino connection failed!");
        connectionCompleted = false;
    }else{

        connectionCompleted = true;
    }

    if(connectionCompleted){

        if(com->readUntil()){
            mtx.lock();

            guiThread->msg.ballX = com->getBallXCoordinate();
            guiThread->msg.ballY = com->getBallYCoordinate();
            guiThread->msg.motorXangle = com->getXMotorAngle();
            guiThread->msg.motorYangle = com->getYMotorAngle();

            QString log;
            log.append("Bx:").append(QString::number(guiThread->msg.ballX)).append("  ");
            log.append("By:").append(QString::number(guiThread->msg.ballY)).append("  ");
            log.append("Sx:").append(QString::number(guiThread->msg.motorXangle)).append("  ");
            log.append("Sy:").append(QString::number(guiThread->msg.motorYangle)).append("  ");
            logger.debug(log.toStdString().c_str());
            logger.debug("Arduino <--> ardThread okeyy");
            qDebug();
            if(isSim3DConnected){
                simThread->msg.ballX = com->getBallXCoordinate();
                simThread->msg.ballY = com->getBallYCoordinate();
                simThread->msg.motorXangle = com->getXMotorAngle();
                simThread->msg.motorYangle = com->getYMotorAngle();
            }
            mtx.unlock();
        }
    }
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
   // connect(&timerServoPlot,SIGNAL(timeout()),this,SLOT(updateServoPlotData()));
   // timerServoPlot.start(0);
}


void MainWindow::updateServoPlotData(){

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    int servoXAngle=guiThread->msg.motorXangle;
    int servoYAngle=guiThread->msg.motorYangle;
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
    if(connectionCompleted){
        ui->textBMsg-> append("Connection is already open.");
        return;
    }

    QString portName = ui->inputPortName->text();
    QString baudRate = ui->inputBaudRate->text();

    ArduinoMessageBean msg;
    msg.portName = portName.toStdString();

    int iBaudRate = baudRate.toInt();
    // set baudrate
    switch(iBaudRate) {
        case 9600: msg.baudRate = SerialPort::BR_9600;
        case 38400: msg.baudRate = SerialPort::BR_38400;
        case 115200: msg.baudRate = SerialPort::BR_115200;
    default: { iBaudRate=9600; msg.baudRate=SerialPort::BR_9600;} // burası ekrana basılabilir
    }

    ui->textBMsg->append("New connection:");
    ui->textBMsg->append(" Port name:"+portName);
    ui->textBMsg->append(" Baud rate:"+QString::number(msg.baudRate));
    ardThread->msg=msg;
    ardThread->start();
}


void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "CSE395 Client",
                                                                tr("Are you sure?\n"),QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        if(isSim3DConnected){
            simThread->stop = true;
            if(server->isEstablished())
                server->close();
        }

        while(ardThread->isRunning()){
            ardThread->terminate();
        }

        while(guiThread->isRunning()){
            guiThread->terminate();
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
        simThread->start();
    }else{
        ui->textBMsg->append("Simulation already runnig");
    }
}

void MainWindow::sim3DConnection(){
    //check connection already established
    if(!isSim3DConnected){
        pid_t pid;
//        pid = fork();
//        if(pid == 0){
//            execl(EXENAME," ");         //start exec of 3d sim
//            exit(EXIT_SUCCESS);
//        }
        if(server == nullptr){
            server = new myTcpServer(this);
            server->listen();
        }
        ui->textBMsg->append("address : " + server->getAddress());
        ui->textBMsg->append("Port number : "+ QString::number(server->getPortNumber()));
        if(server->isEstablished()){       //listen to connect
            isSim3DConnected = true ;
            std::cerr <<  "Connection completed";
        }

    }
    if(isSim3DConnected){
        std::cerr << "Time to write";
        char buffer[30] = "Hello ";
/*        std::sprintf(buffer,"{%d %d %d %d}",simThread->msg.ballX,
                                            simThread->msg.ballY,
                                            simThread->msg.motorXangle,
                                            simThread->msg.motorYangle);*/
        if(server->SendData(buffer))
            std::cerr << "Succesfully sent" << std::endl;
        else
            std::cerr << "Error data sending " << std::endl;
    }

}

void MainWindow::on_btnDisconnect_clicked()
{
    std::cerr<<"on_click_btn_disconnect"<<endl;
    qDebug("test");

    while(ardThread->isRunning()){
        ardThread->terminate();
    }

    while(guiThread->isRunning()){
        guiThread->terminate();
    }

    if(com!=NULL){
        com->closeConnection();
        delete com;
        com=NULL;
    }

    connectionCompleted=false;
    ui->textBMsg->setText("Connection closed!");
}
