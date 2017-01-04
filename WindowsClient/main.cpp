#include <QApplication>
#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Communication.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainWindow w;

  w.show();
  return a.exec();
}


