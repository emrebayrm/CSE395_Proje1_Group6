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

    cerr<<"test2"<<endl;
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    int coordX=150;
    int coordY=200;
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

    /*threadMessage3D message3D;
    threadMessageGrafic messageGrafic;

    int fdGrafic[2];
    pthread_t idGrafic;
    int fd3DSim[2];
    pthread_t id3DSim;

    pipe(fdGrafic); //create pipe between grafik draw.
    //SetMessage
    messageGrafic.pipefd[0] = fdGrafic[0];
    messageGrafic.pipefd[1] = fdGrafic[1];

    int error = pthread_create(&idGrafic,NULL,CommunicateWithGrafic,&messageGrafic);//create thread for Grafic Drawing.

    if(error){
        std::cerr << "Error Creating Grafic thread" << std::endl;
        exit(EXIT_FAILURE);
    }

    pipe(fd3DSim);     //create pipe for 3Dsim
    //Set Message
    message3D.pipefd[0] = fd3DSim[0];
    message3D.pipefd[1] = fd3DSim[1];

    error = pthread_create(&id3DSim,NULL,CommunicateWith3DSim,&message3D);          //create thread for 3Dsim

    if(error){
        std::cerr << "Error Creating 3DSim thread" << std::endl;
        exit(EXIT_FAILURE);
    }
*/
    Communication com(ardThread->msg.portName,ardThread->msg.baudRate);
    if(!com.isCommunicationReady()){
        ardThread->stop=true;
        ui->textBMsg->append("Connection Failed");
    }else{
        guiThread->start();
        char sendBuffer[PACKET_SIZE];
        char getBuffer[6];
        bool sim3DisOpen = false;
        int writeRet;
        while (1) {
            if(com.readUntil()){
                /*guiThread->msg.ballX = com.getBallXCoordinate();
                guiThread->msg.ballY = com.getBallYCoordinate();
                guiThread->msg.motorXangle = com.getXMotorAngle();
                guiThread->msg.motorYangle = com.getBallYCoordinate();*/


            /*sprintf(sendBuffer,PACKETFORMAT,com.getXMotorAngle(),
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
            }//EndRead
        }//End while*/

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


void MainWindow::on_btnConnPlate_clicked()
{
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


    ui->textBMsg->append("Port name:"+portName);
    ui->textBMsg->append("Baud rate:"+QString::number(msg.baudRate));

    /*int error = pthread_create(&thArd,NULL,CommunicateWithArduino,&msg);
    ui->textBMsg->append("Thread Status:"+QString::number(error));
    */
    ardThread->msg=msg;
    ardThread->start();
    //CommunicateWithArduino(msg);

    //guiThread->start();
}
