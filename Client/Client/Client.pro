TEMPLATE = app

QT += qml quick core gui
CONFIG += c++11

SOURCES += main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    SerialPort.cpp \
    Communication.cpp \
    graphicthread.cpp \
    arduinothread.cpp \
    arduinomessagebean.cpp \
    threedsimulationmessage.cpp \
    sim3dthread.cpp \
    mytcpserver.cpp

RESOURCES += res.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT+= widgets printsupport

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    qcustomplot.h \
    SerialPort.h \
    Communication.h \
    graphicthread.h \
    arduinothread.h \
    arduinomessagebean.h \
    threedsimulationmessage.h \
    sim3dthread.h \
    mytcpserver.h

DISTFILES +=

