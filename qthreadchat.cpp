
#include "qthreadchat.h"

QThreadChat::QThreadChat(){
    num_clients = 0;
}

void QThreadChat::start_server(const char *char_ip, const char *char_port){

    char buffer[1024];
    NetConnection::ip_t ip = inet_addr(char_ip);     // This might be upgraded when the class NetConnection have a method to set the ip with a char chain
    NetConnection::port_t port = atoi(char_port);
    server.set_ip(ip);
    server.set_port(port);

    server.init_TCP_server(MAX_TCP_CLIENTS);
    emit(refresh_statuslabel("Server running, waiting for clients..."));
    nclient[num_clients] = server.accept_TCP_connection();
    num_clients++;
    emit(refresh_statuslabel("Client connected"));

    while(1){
        nclient[num_clients-1]->read_packet(buffer, 1024);
        QString msg = QString::fromUtf8(buffer);
        emit(msg_received(msg));
    }

}

void QThreadChat::start_client(const char *char_ip, const char *char_port){

    char buffer[1024];
    NetConnection::ip_t ip = inet_addr(char_ip);     // This might be upgraded when the class NetConnection have a method to set the ip with a char chain
    NetConnection::port_t port = atoi(char_port);
    client.set_ip(ip);
    client.set_port(port);

    emit(refresh_statuslabel("Establishing connection"));
    client.connect_TCP_client();
    emit(refresh_statuslabel("Connection established"));

    while(1){
        client.read_packet(buffer, 1024);
        QString msg = QString::fromUtf8(buffer);
        emit(msg_received(msg));
    }

}















// SERVER

//    QString text_port = ui->PortInput->toPlainText();
//    QString text_ip = ui->IPInput->toPlainText();

//    std::string string_port = text_port.toStdString();      // Conversion QString to string
//    std::string string_ip = text_ip.toStdString();

//    const char *char_port = string_port.c_str();            // Conversion string to char pointer
//    const char *char_ip = string_ip.c_str();

//    NetConnection::ip_t ip = inet_addr(char_ip);     // This might be upgraded when the class NetConnection have a method to set the ip with a char chain
//    NetConnection::port_t port = atoi(char_port);
////GLOBALNetConnection server(ip, port);
//    server.set_ip(ip);
//    server.set_port(port);

//    server.init_TCP_server(MAX_TCP_CLIENTS);
//    ui->StatusLabel->setText("Server running, waiting for clients...");
//    ui->StatusLabel->repaint();                                 // repaint() is necessary because the accept_TCP_connection is blocking,
//    NetConnection *nclient = server.accept_TCP_connection();    // then it blocks the GUI and does not allow the label to change.
////GLOBAL*nclient = server.accept_TCP_connection();
//    ui->StatusLabel->setText("Client connected");
//    ui->StatusLabel->repaint();



// CLIENT

//QString text_port = ui->PortInput->toPlainText();
//QString text_ip = ui->IPInput->toPlainText();

//std::string string_port = text_port.toStdString();      // Conversion QString to string
//std::string string_ip = text_ip.toStdString();

//const char *char_port = string_port.c_str();            // Conversion string to char pointer
//const char *char_ip = string_ip.c_str();

//NetConnection::ip_t ip = inet_addr(char_ip);     // This might be upgraded when the class NetConnection have a method to set the ip with a char chain
//NetConnection::port_t port = atoi(char_port);
////GLOBALNetConnection client(ip, port);
//client.set_ip(ip);
//client.set_port(port);

//ui->StatusLabel->setText("Establishing connection");
//ui->StatusLabel->repaint();
//client.connect_TCP_client();
//ui->StatusLabel->setText("Connection established");
//ui->StatusLabel->repaint();
