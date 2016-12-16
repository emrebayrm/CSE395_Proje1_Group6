#include "mainwindow.h"
#include <QApplication>
#include "Communication.h"
#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "requirements.h"
#include "tools.h"

using namespace Tool;
pthread_t thArduino;
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  w.testThread();
  // burasını normalde baglan diyince baslatacak

  w.show();
  return a.exec();
}


