#include "mainwindow.h"
#include <QApplication>
#include "qthreadchat.h"
// C/C++ Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //w.sthread.start(QThread::LowPriority);

    return a.exec();
}
