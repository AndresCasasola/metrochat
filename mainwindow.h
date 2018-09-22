#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "NetConnection.hpp"
#include "qthreadchat.h"
#include <qthreadtransfer.h>

// C Includes
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// C++ Includes
#include <iostream>

#define MAX_CHAT_LINES                      20
#define MAX_CHAT_CHARS                      101
#define APP_TYPE_SERVER                     0
#define APP_TYPE_CLIENT                     1
#define KEY_TO_SEND                         Qt::Key_F1
#define KEY_TO_SEND_2                       Qt::Key_F12
#define DEFAULT_SERVER_DESTINY_PATH         "/home/andres/metrochat_files/serverfiles/file"
#define DEFAULT_CLIENT_DESTINY_PATH         "/home/andres/metrochat_files/clientfiles"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // User Interface
    void on_pushButton_clicked();
    void on_SendButton_clicked();
    void on_ServerButton_clicked();
    void on_ClientButton_clicked();
    void on_ConnectionButton_clicked();
    void on_ConnectionButton_2_clicked();
    void on_RouteButton_clicked();
    void on_SendFileButton_clicked();
    void on_ClientButton_2_clicked();
    void on_ServerButton_2_clicked();
    void on_RouteButton_2_clicked();
    void on_ResetButton_clicked();
    void on_OpenFolderButton_clicked();
    void on_HelpButton_clicked();
    void on_pushButton_2_clicked();

    // QThreadChat
    void refresh_statuslabel(QString string);
    void msg_received(QString);

    // QThreadTransfer
    void refresh_statuslabel_2(QString string);
    void refresh_statuslabel_3(QString string);
    void ask_destiny_path();
    void incomming_file(QString string, int size);
    void refresh_progressBar(int value);

    // Events
    void keyPressEvent(QKeyEvent *event);

signals:
    // QThreadChat
    void start_server(const char *ip, const char *port);
    void start_client(const char *ip, const char *port);
    // QThreadTransfer
    void start_transfer_server(const char *ip, const char *port);
    void start_transfer_client(const char *ip, const char *port);
    void process_transfer_server(QString destiny_path);
    void process_transfer_client(QString destiny_path);

private:
    Ui::MainWindow *ui;

    // Global variables
    QString chat[MAX_CHAT_LINES];   // Chat to show in the text output
    int msg_counter;                // That variable saves the current number of the output chat lines
    int app_type;                   // That variable saves if the application is working as server or client
    bool help_state;                // That variable saves if the application is showing help or not

    // Chat
    QThread thread;
    QThreadChat chatThread;

    //File Transfer
    QThread thread2;
    QThreadTransfer transferThread;

    //QString msg_owners[MAX_CHAT_LINES];

};

#endif // MAINWINDOW_H
