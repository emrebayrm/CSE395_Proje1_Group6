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


    QImage img;
    img.loadFromData("/home/hasan/workspace/CSE395_Proje1_Group6/Client/Client/gtuLogo500.png");
    img = img.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->gtuLogo->setPixmap(QPixmap::fromImage(img));


    guiThread = new GraphicThread(this);
    ardThread = new ArduinoThread(this);

    // guiTHreadi icinde startThread calistirilinca, bu clasın verilen metodunu calistir
    connect(guiThread,SIGNAL(startThread()),this,SLOT(updateServoPlotData()));
    connect(guiThread,SIGNAL(startThread()),this,SLOT(updateXYPlotData()));

    connect(ardThread,SIGNAL(startArdThread()),this, SLOT(ardConnection()));

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
            .arg(coordX)
            .arg(coordY));

}

void MainWindow::ardConnection()
{

    if(!connectionCompleted)
        com = new Communication(ardThread->msg.portName,ardThread->msg.baudRate,&mtx);
    if(!com->isCommunicationReady()){
        ardThread->stop=true;
        ui->textBMsg->append("Connection Failed");
        connectionCompleted = false;
    }else{
        guiThread->start();
        connectionCompleted = true;
    }
    if(connectionCompleted){
        if(com->readUntil()){
            mtx.lock();
            //cerr<<"inmutexmain"<<endl;
            guiThread->msg.ballX = com->getBallXCoordinate();
            guiThread->msg.ballY = com->getBallYCoordinate();
            guiThread->msg.motorXangle = com->getXMotorAngle();
            guiThread->msg.motorYangle = com->getYMotorAngle();

            std::cerr<<"BallX:"<<guiThread->msg.ballX<<"BallY"<<guiThread->msg.ballY<<endl;
            std::cerr<<"ServoX:"<<guiThread->msg.motorXangle<<"ServoY:"<<guiThread->msg.motorYangle<<endl;
            //cerr<<"outmutexmain"<<endl;
            mtx.unlock();
        }
    }
    /*

            sprintf(sendBuffer,PACKETFORMAT,com.getXMotorAngle(),
                                             com.getYMotorAngle(),
                                             com.getBallXCoordinate(),
                                             com.getBallYCoordinate());

            std::cerr << com.getBallXCoordinate()  << std::endl;
            writeRet = write(fdGrafic[1],sendBuffer,sizeof(char)*PACKET_SIZE);   //Send packet to grafik pipe.
            if(writeRet < 0){
                std::cerr << "Error Writing " << std::endl;
                exit(EXIT_FAILURE);
            }
            read(fd3DSim[0],getBuffer,THREADCOMSIZE);

            //check button unreal button has pressed.
            if(std::strcmp(getBuffer,PRESSED) == 0){
                //TODO:Pressed Section
                sim3DisOpen = true;
            }

            //check 3D is open if openned check if it is ready Message
            if(sim3DisOpen && (std::strcmp(getBuffer,READY)) == 0){
                writeRet = write(fd3DSim[1],sendBuffer,sizeof(char)*PACKET_SIZE);//Send Packet to 3d

                if(writeRet < 0){
                    std::cerr << "Write Error. EXITING !!!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            //check 3D sim is quitted ?
            if(std::strcmp(getBuffer,QUIT) == 0){
                //pthread_join(id3DSim,NULL);
                sim3DisOpen = false;
            }

            //Send To Grafic Thread
            writeRet = write(fdGrafic[1],sendBuffer,sizeof(char)*PACKET_SIZE);
            */
       //     }//EndRead
     //   }//End while*/
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
          .arg(servoXAngle)
          .arg(servoYAngle));

}


void MainWindow::on_btnConnPlate_clicked()
{
    if(connectionCompleted){
        ui->textBMsg-> append("Already Connected");
        return;
    }

    QString portName = ui->inputPortName->text();
    QString baudRate = ui->inputBaudRate->text();

    ArduinoMessageBean msg;
    msg.portName = portName.toStdString();

    int iBaudRate = baudRate.toInt();
    // set baudrate
    switch(iBaudRate) {
        case 4800: msg.baudRate = SerialPort::BR_4800;
        case 9600: msg.baudRate = SerialPort::BR_9600;
        case 38400: msg.baudRate = SerialPort::BR_38400;
        case 115200: msg.baudRate = SerialPort::BR_115200;
    default: { iBaudRate=9600; msg.baudRate=SerialPort::BR_9600;} // burası ekrana basılabilir
    }

    if(ardThread->isRunning()){
        ui->textBMsg->append("Arduino thread already works");
    }else{
        ui->textBMsg->append("Port name:"+portName);
        ui->textBMsg->append("Baud rate:"+QString::number(msg.baudRate));
        ardThread->msg=msg;
        ardThread->start();
    }

    /*int error = pthread_create(&thArd,NULL,CommunicateWithArduino,&msg);
    ui->textBMsg->append("Thread Status:"+QString::number(error));
    */

}


void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "CSE395 Client",
                                                                tr("Are you sure?\n"),QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        guiThread->stop=true;
        ardThread->stop=true;
        sleep(1);
        event->accept();
    }
}
