#ifndef QTHREADTRANSFER_H
#define QTHREADTRANSFER_H

#include <QObject>
#include <QtCore>
#include <QMainWindow>
#include "NetConnection.hpp"
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
#include <unistd.h>
#include <signal.h>
#include <QMessageBox>

#define MAX_TCP_CLIENTS 1
#define MAX_FILENAME_SIZE 512

class QThreadTransfer : public QObject{

    Q_OBJECT

public:
    explicit QThreadTransfer(QObject *parent = nullptr);

signals:
    void refresh_statuslabel_2(QString string);
    void refresh_statuslabel_3(QString string);
    void ask_destiny_path();
    void incomming_file(QString string, int size);
    void refresh_progressBar(int value);

private slots:
    void start_transfer_server(const char *ip, const char *port);
    void start_transfer_client(const char *ip, const char *port);
    void process_transfer_server(QString destiny_path);
    void process_transfer_client(QString destiny_path);

public:
    NetConnection server, client;
    NetConnection *nclient[MAX_TCP_CLIENTS];
    int num_clients;
    QMutex mutex;
    int decision;

};

#endif // QTHREADTRANSFER_H
