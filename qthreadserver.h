#ifndef QTHREADSERVER_H
#define QTHREADSERVER_H

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

#define MAX_TCP_CLIENTS 1

class QThreadServer : public QObject{

    Q_OBJECT

public:
    QThreadServer();

signals:
    void send_button();
    void refresh_statuslabel(QString string);
    void msg_received(QString);

private slots:
    void start_server(const char *char_ip, const char *char_port);
    void start_client(const char *char_ip, const char *char_port);


public:
    NetConnection server, client;
    NetConnection *nclient[MAX_TCP_CLIENTS];
    int num_clients;


};

#endif // QTHREADSERVER_H
