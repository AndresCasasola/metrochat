#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    ui->StatusLabel->setText("None");
    msg_counter = 0;
    app_type = APP_TYPE_SERVER;
    ui->RouteOutput_2->setText(DEFAULT_SERVER_DESTINY_PATH);

    // Create folders to receive files
    //open("/home/andres/metrochat_files/serverfiles", O_WRONLY | O_CREAT | O_TRUNC);   // Does not work...
    //open("/home/andres/metrochat_files/clientfiles", O_WRONLY | O_CREAT | O_TRUNC);

    help_state = 0;
    ui->HelpLabel_1->hide();
    ui->HelpLabel_2->hide();
    ui->HelpLabel_1->setStyleSheet("QLabel { color : red; }");
    ui->HelpLabel_2->setStyleSheet("QLabel { color : red; }");

    ui->RouteOutput->setText("/home/andres/Pictures/video");

    // QThreadChat
    connect(this, SIGNAL(start_server(const char*,const char*)), &chatThread, SLOT(start_server(const char*,const char*)));
    connect(this, SIGNAL(start_client(const char*,const char*)), &chatThread, SLOT(start_client(const char*,const char*)));
    connect(&chatThread, SIGNAL(refresh_statuslabel(QString)), this, SLOT(refresh_statuslabel(QString)));
    connect(&chatThread, SIGNAL(msg_received(QString)), this, SLOT(msg_received(QString)));
    // QThreadTransfer
    connect(this, SIGNAL(start_transfer_server(const char*,const char*)), &transferThread, SLOT(start_transfer_server(const char*,const char*)));
    connect(this, SIGNAL(start_transfer_client(const char*,const char*)), &transferThread, SLOT(start_transfer_client(const char*,const char*)));
    connect(&transferThread, SIGNAL(refresh_statuslabel_2(QString)), this, SLOT(refresh_statuslabel_2(QString)));
    connect(&transferThread, SIGNAL(refresh_statuslabel_3(QString)), this, SLOT(refresh_statuslabel_3(QString)));
    connect(this, SIGNAL(process_transfer_server(QString)), &transferThread, SLOT(process_transfer_server(QString)));
    connect(this, SIGNAL(process_transfer_client(QString)), &transferThread, SLOT(process_transfer_client(QString)));
    connect(&transferThread, SIGNAL(ask_destiny_path()), this, SLOT(ask_destiny_path()));
    connect(&transferThread, SIGNAL(incomming_file(QString,int)), this, SLOT(incomming_file(QString,int)));
    connect(&transferThread, SIGNAL(refresh_progressBar(int)), this, SLOT(refresh_progressBar(int)));
    //connect(ui->TextInput, SIGNAL(QPlainTextEdit::keyPressEvent(QKeyEvent *)), this, SLOT(QPlainTextEdit::keyPressEvent(QKeyEvent *)));

}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::on_pushButton_clicked(){

    QString text = "This is metrochat application!  ----> Development version";
    ui->statusBar->showMessage(text, 0);

}

void MainWindow::refresh_statuslabel(QString string){

    ui->StatusLabel->setText(string);
    ui->StatusLabel->repaint();

}

void MainWindow::refresh_statuslabel_2(QString string){

    ui->StatusLabel_2->setText(string);
    ui->StatusLabel_2->repaint();

}

void MainWindow::refresh_statuslabel_3(QString string){

    if(string == "Rejected"){
        QMessageBox::information(this, "Information", "Rejected file");
    }
    ui->StatusLabel_3->setText(string);
    ui->StatusLabel_3->repaint();

}

void MainWindow::keyPressEvent(QKeyEvent *event){

    if(event->key() == KEY_TO_SEND || event->key() == KEY_TO_SEND_2){
        ui->SendButton->clicked(true);
    }
}

void MainWindow::on_SendButton_clicked(){

    QString input_raw_text = ui->TextInput->toPlainText() + '\n';
    QString input_text = ui->UserInput->toPlainText() + ": " + ui->TextInput->toPlainText() + '\n';
    std::string string_text = input_text.toStdString();
    char char_text[1024];
    strcpy(char_text, string_text.c_str());
    ui->TextInput->clear();

    if(app_type == APP_TYPE_SERVER){

        chatThread.nclient[(chatThread.num_clients)-1]->write_packet(char_text, strlen(char_text));
    }

    if(app_type == APP_TYPE_CLIENT){

        chatThread.client.write_packet(char_text, strlen(char_text));

    }

    if(msg_counter == MAX_CHAT_LINES){
        for(int i=0; i<MAX_CHAT_LINES-1; i++){
            chat[i] = chat[i+1];
        }
        msg_counter--;
    }

    QString chat_toprint;
    chat[msg_counter] = "Me: " + input_raw_text;
    for(int i=chat[msg_counter].size(); i<MAX_CHAT_CHARS; i++){
        chat[msg_counter] = " " + chat[msg_counter];
    }

    msg_counter++;
    for(int i=0; i<msg_counter; i++){
        chat_toprint += chat[i];
    }
    ui->ChatOutput->setText(chat_toprint);
    ui->TextInput->clear();

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

void MainWindow::on_ServerButton_2_clicked(){

    app_type = APP_TYPE_SERVER;
    ui->ServerButton_2->setDisabled(1);
    ui->ClientButton_2->setEnabled(1);
    ui->ClientButton_2->setChecked(0);
    ui->ServerClientLabel_2->setText("Working as server");
    ui->RouteOutput_2->setText(DEFAULT_SERVER_DESTINY_PATH);

}

void MainWindow::on_ClientButton_2_clicked(){

    app_type = APP_TYPE_CLIENT;
    ui->ServerButton_2->setEnabled(1);
    ui->ClientButton_2->setDisabled(1);
    ui->ServerButton_2->setChecked(0);
    ui->ServerClientLabel_2->setText("Working as client");
    ui->RouteOutput_2->setText(DEFAULT_CLIENT_DESTINY_PATH);

}

void MainWindow::on_ConnectionButton_clicked(){

    if(app_type == APP_TYPE_SERVER){

        QString text_port = ui->PortInput->toPlainText();
        QString text_ip = ui->IPInput->toPlainText();

        std::string string_port = text_port.toStdString();      // Conversion QString to string
        std::string string_ip = text_ip.toStdString();

        const char *char_port = string_port.c_str();            // Conversion string to char pointer
        const char *char_ip = string_ip.c_str();

        chatThread.moveToThread(&thread);
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

        chatThread.moveToThread(&thread);
        thread.start();
        emit(start_client(char_ip, char_port));

    }
}

void MainWindow::on_ConnectionButton_2_clicked(){

    if(app_type == APP_TYPE_SERVER){

        QString text_port = ui->PortInput_2->toPlainText();
        QString text_ip = ui->IPInput_2->toPlainText();

        std::string string_port = text_port.toStdString();      // Conversion QString to string
        std::string string_ip = text_ip.toStdString();

        const char *char_port = string_port.c_str();            // Conversion string to char pointer
        const char *char_ip = string_ip.c_str();

        transferThread.moveToThread(&thread2);
        thread2.start();
        emit(start_transfer_server(char_ip, char_port));

    }

    if(app_type == APP_TYPE_CLIENT){

        QString text_port = ui->PortInput_2->toPlainText();
        QString text_ip = ui->IPInput_2->toPlainText();

        std::string string_port = text_port.toStdString();      // Conversion QString to string
        std::string string_ip = text_ip.toStdString();

        const char *char_port = string_port.c_str();            // Conversion string to char pointer
        const char *char_ip = string_ip.c_str();

        transferThread.moveToThread(&thread2);
        thread2.start();
        emit(start_transfer_client(char_ip, char_port));

    }

}

void MainWindow::on_RouteButton_clicked(){

    QString file_path = QFileDialog::getOpenFileName(this, "Select file", "/home/andres/Pictures");
    //QMessageBox::information(this, "Selected file", file_route);
    ui->RouteOutput->setText(file_path);

}

void MainWindow::on_ResetButton_clicked(){

    ui->RouteOutput->clear();
    ui->progressBar->setValue(0);
    ui->StatusLabel_3->setText("None");
    ui->StatusLabel_3->repaint();
}

void MainWindow::on_RouteButton_2_clicked(){

    //QString file_path = QFileDialog::getOpenFileName(this, "Select destiny path", "/home/andres/Pictures");
    QString directory_path = QFileDialog::getExistingDirectory(this, "Select destiny path", "/home/andres/Pictures");
    //QMessageBox::information(this, "Selected file", file_route);
    ui->RouteOutput_2->setText(directory_path);

}

void MainWindow::on_OpenFolderButton_clicked(){

    QString path = ui->RouteOutput_2->toPlainText();
    std::string command = "nautilus " + path.toStdString() + "&";
    system(command.c_str());
}

void MainWindow::on_HelpButton_clicked(){

    if(help_state){
        help_state=0;
        ui->HelpLabel_1->hide();
        ui->HelpLabel_2->hide();
    }else{
        help_state=1;
        ui->HelpLabel_1->show();
        ui->HelpLabel_2->show();
    }
}

void MainWindow::ask_destiny_path(){

    QString path = ui->RouteOutput_2->toPlainText();

    if(path.size() == 0){
        QMessageBox::information(this, "Alert!", "No destiny path selected.");
        sleep(1);
    }

    QString destiny_path = ui->RouteOutput_2->toPlainText();

    if(app_type == APP_TYPE_SERVER){

        emit(process_transfer_server(destiny_path));
    }

    if(app_type == APP_TYPE_CLIENT){

        emit(process_transfer_client(destiny_path));
    }
}

void MainWindow::refresh_progressBar(int value){
    ui->progressBar->setValue(value);
}

void MainWindow::incomming_file(QString string, int size){

    QString unit = "B";
    double fsize = size;
    printf("Size: %f\n", fsize);
    fflush(stdout);

    if( (size/1000000000) > 0){
        fsize /= 1000000000;
        unit = "GB";
    }else if( (size/1000000) > 0){
        fsize /= 1000000;
        unit = "MB";
    }else if( (size/1000) > 0){
        fsize /= 1000;
        unit = "KB";
    }

    char size_char[sizeof(int)];
    sprintf(size_char, "%.1f", fsize);
    QString size_qstring = QString::fromStdString(std::string(size_char));
    QString msg = "Filename:  " + string + "\n" + "Size:          " + size_qstring + " " + unit;
    //QMessageBox::information(this, "Incomming file", message);

    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(msg);
    msgBox.setWindowTitle("Incomming file");
    msgBox.setInformativeText("Save it?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Save:
        transferThread.decision = 1;
        break;
      case QMessageBox::Cancel:
        transferThread.decision = 0;
        break;
      default:
        transferThread.decision = 1;
        break;
    }
    transferThread.mutex.unlock();

}

void MainWindow::on_SendFileButton_clicked(){

    if(app_type == APP_TYPE_SERVER){

        char buffer[1024];
        int bytes_read = 0;
        double total_bytes_read = 0;
        QString file_path = ui->RouteOutput->toPlainText();
        const char *file_path_char = file_path.toUtf8();

        int fd = open(file_path_char, O_RDONLY);
        if(fd < 0){
            perror("Error opening file");
            exit(1);
        }

        // Sending file size
        struct stat file_data;
        int v = fstat(fd, &file_data);
        if(v < 0){
            perror("Error reading file stat");
            exit(1);
        }
        printf("File size: %d\n", file_data.st_size);
        fflush(stdout);
        int size=file_data.st_size;
        transferThread.nclient[(transferThread.num_clients)-1]->write_packet((char *)&size, sizeof(int));

        // Sending file name
        std::string filename;
        std::string file_path_string = file_path.toStdString();
        int path_size = file_path_string.size();
        char c;
        int i = path_size;
        printf("Path size: %d\n", path_size);
        fflush(stdout);
        while(c != '/'){
            c = file_path_string[i-1];
            i--;
        }
        printf("Name size: %d\n", (path_size-1)-i);
        fflush(stdout);
        for(int j=0; j<(path_size-1)-i; j++){
            filename[j] = file_path_string[(i+1)+j];
        }
        const char *filename_char = (char *) malloc(MAX_FILENAME_SIZE);
        filename_char = filename.c_str();
        printf("File name: %s, Char size: %d\n", filename_char, strlen(filename_char));
        fflush(stdout);
        int namesize = strlen(filename_char);
        transferThread.nclient[(transferThread.num_clients)-1]->write_packet((char *)&namesize, sizeof(int));
        transferThread.nclient[(transferThread.num_clients)-1]->write_packet((char *)filename_char, namesize);

            // Sending file
            bytes_read = 0;
            ui->StatusLabel_3->setText("Transmitting...");
            do{
                bytes_read = read(fd, buffer, 1024);
                if(bytes_read < 0){
                    perror("Error reading file");
                    exit(1);
                }else if(bytes_read > 0){
                    transferThread.nclient[(transferThread.num_clients)-1]->write_packet(buffer, bytes_read);
                }

                // Show progress status
                total_bytes_read += bytes_read;
                int progress_percent = 100 * total_bytes_read / size;
                //printf("(%d Bytes)Progress percent: %d\n", total_bytes_read, progress_percent);
                //fflush(stdout);
                ui->progressBar->setValue(progress_percent);

            }while(bytes_read > 0);
            ui->StatusLabel_3->setText("Transmitted");
            printf("Transmission completed\n");
            fflush(stdout);
    }

    if(app_type == APP_TYPE_CLIENT){

        char buffer[1024];
        int bytes_read;
        QString file_route = ui->RouteOutput->toPlainText();
        const char *file_route_char = file_route.toUtf8();

        int fd = open(file_route_char, O_RDONLY);
        if(fd < 0){
            perror("Error opening file");
            exit(1);
        }

        do{
            bytes_read = read(fd, buffer, 1024);
            if(bytes_read < 0){
                perror("Error reading file");
                exit(1);
            }else if(bytes_read > 0){
                //transferThread.client.write_packet((char *)&bytes_read, sizeof(int));
                transferThread.client.write_packet(buffer, bytes_read);
            }
        }while(bytes_read > 0);

        //transferThread.client.write_packet();

    }

}

void MainWindow::on_pushButton_2_clicked(){
    ui->PortInput_2->setPlainText("25005");
}






/* OPERATIVE VERSION
void MainWindow::on_SendFileButton_clicked(){

    if(app_type == APP_TYPE_SERVER){

        char buffer[1024];
        int bytes_read = 0;
        int total_bytes_read = 0;
        QString file_route = ui->RouteOutput->toPlainText();
        const char *file_route_char = file_route.toUtf8();

        int fd = open(file_route_char, O_RDONLY);
        if(fd < 0){
            perror("Error opening file");
            exit(1);
        }

        struct stat file_data;
        int v = fstat(fd, &file_data);
        printf("File size: %d\n", file_data.st_size);
        fflush(stdout);

        do{
            bytes_read = read(fd, buffer, 1024);
            if(bytes_read < 0){
                perror("Error reading file");
                exit(1);
            }else if(bytes_read > 0){
                //transferThread.nclient[(transferThread.num_clients)-1]->write_packet((char *)&bytes_read, sizeof(int));
                transferThread.nclient[(transferThread.num_clients)-1]->write_packet(buffer, bytes_read);
            }

            // Show progress status
            total_bytes_read += bytes_read;
            int progress_percent = 100 * total_bytes_read / file_data.st_size;
            printf("(%d Bytes)Progress percent: %d\n", total_bytes_read, progress_percent);
            fflush(stdout);
            ui->progressBar->setValue(progress_percent);

        }while(bytes_read > 0);

        //transferThread.nclient[(transferThread.num_clients)-1]->write_packet();
    }

    if(app_type == APP_TYPE_CLIENT){

        char buffer[1024];
        int bytes_read;
        QString file_route = ui->RouteOutput->toPlainText();
        const char *file_route_char = file_route.toUtf8();

        int fd = open(file_route_char, O_RDONLY);
        if(fd < 0){
            perror("Error opening file");
            exit(1);
        }

        do{
            bytes_read = read(fd, buffer, 1024);
            if(bytes_read < 0){
                perror("Error reading file");
                exit(1);
            }else if(bytes_read > 0){
                //transferThread.client.write_packet((char *)&bytes_read, sizeof(int));
                transferThread.client.write_packet(buffer, bytes_read);
            }
        }while(bytes_read > 0);

        //transferThread.client.write_packet();

    }

}*/


