#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "NetConnection.hpp"
#include "qthreadserver.h"

// C Includes
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// C++ Includes
#include <iostream>

#define MAX_CHAT_LINES 20
#define APP_TYPE_SERVER 0
#define APP_TYPE_CLIENT 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QThreadServer sthread;

private slots:
    void on_pushButton_clicked();
    void on_SendButton_clicked();
    void on_ServerButton_clicked();
    void on_ClientButton_clicked();
    void on_ConnectionButton_clicked();
    void refresh_statuslabel(QString string);
    void msg_received(QString);

signals:
    void start_server(const char *ip, const char *port);
    void start_client(const char *ip, const char *port);
    void signal_proof();

private:
    Ui::MainWindow *ui;

    // Global variables
    QString chat[MAX_CHAT_LINES];   // Chat to show in the text output
    int msg_counter;                // That variable saves the current number of the output chat lines
    int app_type;                   // That variable saves if the application is working as server or client
    QThread thread;

};

#endif // MAINWINDOW_H
