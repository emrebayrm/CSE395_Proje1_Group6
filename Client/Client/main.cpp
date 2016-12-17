#include "mainwindow.h"
#include <QApplication>
#include "Communication.h"
#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "requirements.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  w.show();
  return a.exec();
}


