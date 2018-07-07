#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    msg_counter = 0;
    app_type = APP_TYPE_SERVER;
    connect(this, SIGNAL(start_server(const char*,const char*)), &sthread, SLOT(start_server(const char*,const char*)));
    connect(this, SIGNAL(start_client(const char*,const char*)), &sthread, SLOT(start_client(const char*,const char*)));
    connect(&sthread, SIGNAL(refresh_statuslabel(QString)), this, SLOT(refresh_statuslabel(QString)));
    connect(&sthread, SIGNAL(msg_received(QString)), this, SLOT(msg_received(QString)));

}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::on_pushButton_clicked(){

    QString text = " This is metrochat application!  ----> Development version";
    ui->statusBar->showMessage(text, 0);

}

void MainWindow::refresh_statuslabel(QString string){

    ui->StatusLabel->setText(string);
    ui->StatusLabel->repaint();

}

void MainWindow::on_SendButton_clicked(){

    if(app_type == APP_TYPE_SERVER){

        QString input_text = ui->TextInput->toPlainText() + '\n';
        std::string string_text = input_text.toStdString();
        char char_text[1024];
        strcpy(char_text, string_text.c_str());
        ui->TextInput->clear();
        sthread.nclient[(sthread.num_clients)-1]->write_packet(char_text, strlen(char_text));
    }

    if(app_type == APP_TYPE_CLIENT){

        QString input_text = ui->TextInput->toPlainText() + '\n';
        std::string string_text = input_text.toStdString();
        char char_text[1024];
        strcpy(char_text, string_text.c_str());
        ui->TextInput->clear();
        sthread.client.write_packet(char_text, strlen(char_text));

    }
}

void MainWindow::msg_received(QString msg){

    if(msg_counter == MAX_CHAT_LINES){
        for(int i=0; i<MAX_CHAT_LINES-1; i++){
            chat[i] = chat[i+1];
        }
        msg_counter--;
    }

    QString chat_toprint;

    chat[msg_counter] = msg;
    msg_counter++;

    for(int i=0; i<msg_counter; i++){
        chat_toprint += chat[i];
    }

    ui->ChatOutput->setText(chat_toprint);
    ui->TextInput->clear();

}

void MainWindow::on_ServerButton_clicked(){

    app_type = APP_TYPE_SERVER;
    ui->ServerButton->setDisabled(1);
    ui->ClientButton->setEnabled(1);
    ui->ClientButton->setChecked(0);
    ui->ServerClientLabel->setText("Working as server");

}

void MainWindow::on_ClientButton_clicked(){

    app_type = APP_TYPE_CLIENT;
    ui->ServerButton->setEnabled(1);
    ui->ClientButton->setDisabled(1);
    ui->ServerButton->setChecked(0);
    ui->ServerClientLabel->setText("Working as client");

}

void MainWindow::on_ConnectionButton_clicked(){

    if(app_type == APP_TYPE_SERVER){

        QString text_port = ui->PortInput->toPlainText();
        QString text_ip = ui->IPInput->toPlainText();

        std::string string_port = text_port.toStdString();      // Conversion QString to string
        std::string string_ip = text_ip.toStdString();

        const char *char_port = string_port.c_str();            // Conversion string to char pointer
        const char *char_ip = string_ip.c_str();

        sthread.moveToThread(&thread);
        thread.start();
        emit(start_server(char_ip, char_port));

    }

    if(app_type == APP_TYPE_CLIENT){

        QString text_port = ui->PortInput->toPlainText();
        QString text_ip = ui->IPInput->toPlainText();

        std::string string_port = text_port.toStdString();      // Conversion QString to string
        std::string string_ip = text_ip.toStdString();

        const char *char_port = string_port.c_str();            // Conversion string to char pointer
        const char *char_ip = string_ip.c_str();

        sthread.moveToThread(&thread);
        thread.start();
        emit(start_client(char_ip, char_port));

    }
}




