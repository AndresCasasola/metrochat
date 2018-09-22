
#include "qthreadtransfer.h"

QThreadTransfer::QThreadTransfer(QObject *parent) : QObject(parent){
    num_clients = 0;
}

void QThreadTransfer::start_transfer_server(const char *char_ip, const char *char_port){

    emit(refresh_statuslabel_2("Working..."));

    char buffer[1024];
    NetConnection::ip_t ip = inet_addr(char_ip);     // This might be upgraded when the class NetConnection have a method to set the ip with a char chain
    NetConnection::port_t port = atoi(char_port);
    server.set_ip(ip);
    server.set_port(port);

    server.init_TCP_server(MAX_TCP_CLIENTS);
    emit(refresh_statuslabel_2("Server running, waiting for clients..."));
    nclient[num_clients] = server.accept_TCP_connection();
    num_clients++;
    emit(refresh_statuslabel_2("Client connected"));

    emit(ask_destiny_path());

}

void QThreadTransfer::start_transfer_client(const char *char_ip, const char *char_port){

    char buffer[1024];
    NetConnection::ip_t ip = inet_addr(char_ip);     // This might be upgraded when the class NetConnection have a method to set the ip with a char chain
    NetConnection::port_t port = atoi(char_port);
    client.set_ip(ip);
    client.set_port(port);

    emit(refresh_statuslabel_2("Establishing connection"));
    client.connect_TCP_client();
    emit(refresh_statuslabel_2("Connection established"));

    emit(ask_destiny_path());

}

void QThreadTransfer::process_transfer_server(QString destiny_path){

    char buffer[1024];
    int bytes_read, bytes_writen;

    int fd = open(destiny_path.toStdString().c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    if(fd < 0){
        perror("Error opening file");
        exit(1);
    }

    while(1){
        do{
            bytes_read = nclient[num_clients-1]->read_packet(buffer, 1024);
            if(bytes_read < 0){
                perror("Error reading file");
                exit(1);
            }else if(bytes_read > 0){
                bytes_writen = write(fd, buffer, bytes_read);
            }
        }while(bytes_read > 0);
    }
}

void QThreadTransfer::process_transfer_client(QString destiny_path){

    char buffer[1024];
    int bytes_read, bytes_writen;
    double total_bytes_read;

    while(1){

        // Read file size
        int size=0;
        client.read_packet((char *)&size, sizeof(int));
        printf("File size: %d\n", size);
        fflush(stdout);

        // Read file name
        char filename[MAX_FILENAME_SIZE];
        int namesize;
        client.read_packet((char *)&namesize, sizeof(int));
        printf("File name size: %d\n", namesize);
        fflush(stdout);
        client.read_packet(filename, namesize);
        printf("File name: %s\n", filename);
        fflush(stdout);

        // Asking to save file
        QString filename_qstring = QString::fromStdString(std::string(filename));
        decision=0;
        emit(incomming_file(filename_qstring, size));

        // Waiting the MessageBox response
        mutex.lock();
        mutex.lock();

        if(decision){
            // Creating file and opening it
            std::string filepath_string = destiny_path.toStdString() + std::string("/") + std::string(filename);
            printf("File path : %s\n", filepath_string.c_str());
            fflush(stdout);
            int fd = creat(filepath_string.c_str(), S_IRWXU | S_IRWXG);
            if(fd < 0){
                perror("Error opening file");
                exit(1);
            }
            // Read file
            bytes_read = 0;
            emit(refresh_statuslabel_3("Receiving..."));
            do{
                bytes_read = client.read_packet(buffer, 1024);
                if(bytes_read < 0){
                    perror("Error reading file");
                    exit(1);
                }else if(bytes_read > 0){
                    bytes_writen = write(fd, buffer, bytes_read);
                }
                total_bytes_read += bytes_read;
                // Show progress status
                int progress_percent = 100 * total_bytes_read / size;
                //printf("(%d Bytes)Progress percent: %d\n", total_bytes_read, progress_percent);
                //fflush(stdout);
                emit(refresh_progressBar(progress_percent));
            }while(total_bytes_read < size);
            emit(refresh_statuslabel_3("Received"));
            printf("Reception completed\n");
            fflush(stdout);
            close(fd);
        }else{
            // Reject all data
            emit(refresh_statuslabel_3("Rejected"));
            do{
                bytes_read = client.read_packet(buffer, 1024);
            }while(bytes_read > 0);
        }
    }
}



/* OPERATIVE VERSION
void QThreadTransfer::process_transfer_client(QString destiny_path){

    char buffer[1024];
    int bytes_read, bytes_writen;

    int fd = open(destiny_path.toStdString().c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    if(fd < 0){
        perror("Error opening file");
        exit(1);
    }

    while(1){
        do{
            bytes_read = client.read_packet(buffer, 1024);
            if(bytes_read < 0){
                perror("Error reading file");
                exit(1);
            }else if(bytes_read > 0){
                bytes_writen = write(fd, buffer, bytes_read);
            }
        }while(bytes_read > 0);
    }
}
*/
