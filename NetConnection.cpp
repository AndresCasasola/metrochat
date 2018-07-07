
#include "NetConnection.hpp"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

using namespace std;

    NetConnection::NetConnection(){
        ip=0;
        port=0;
        sd=0;
    }

    NetConnection::NetConnection(port_t new_port){
        port = new_port;
    }

    NetConnection::NetConnection(ip_t new_ip, port_t new_port){
        ip=new_ip;
        port=new_port;
    }

    NetConnection::NetConnection(string domain_name, port_t new_port){
        // Get ip from domain
        // ip=ip_got_from_domain
        port=new_port;
    }

    NetConnection::~NetConnection(){}

    void NetConnection::set_socket(int new_sd){
        sd=new_sd;
    }

	int NetConnection::get_socket(){
        return sd;
    }

    void NetConnection::set_ip(ip_t new_ip){
        ip=new_ip;
    }

    NetConnection::ip_t NetConnection::get_ip(){
        return ip;
    }
		
    void NetConnection::set_port(port_t new_port){
        port=new_port;
    }

    NetConnection::port_t NetConnection::get_port(){
        return port;
    }

    void NetConnection::set_domain_name(std::string domain_name){
        // No domain_name attribute is in the class
    }

    void NetConnection::init_TCP_server(uint8_t max_clients){
        sd=socket(PF_INET, SOCK_STREAM, 0);
        if(sd < 0){
            perror("(!)Error setting TCP socket");
            exit(1);
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    
        if (bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("(!)Error binding socket");
            exit(1);
        }

        uint8_t l = listen(sd, max_clients);
        if(l < 0){
            perror("(!)Error setting TCP socket as listener server");
            exit(1); 
        }
    }

    void NetConnection::connect_TCP_client(){
        sd=socket(PF_INET, SOCK_STREAM, 0);
        if(sd < 0){
            perror("(!)Error setting TCP socket");
            exit(1);
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = ip;       // htonl(ip)?? It does not work...
    
        int c = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
        if(c < 0){
            perror("(!)Error connecting to server");
            exit(1);
        }
    }

    NetConnection* NetConnection::accept_TCP_connection(){

        NetConnection *client = new NetConnection;
        int nclient_sd;
        struct sockaddr_in client_addr;
        socklen_t client_len;

        client_len = sizeof(client_addr);
        nclient_sd = accept(sd, (struct sockaddr *) &client_addr, &client_len);
        if(nclient_sd < 0){
            perror("(!)Error accepting request from client");
            exit(1);
        }

        ip_t client_ip = client_addr.sin_addr.s_addr;   	// ip_t:   32 bits <-> s_addr:   32 bit
        port_t client_port = ntohs(client_addr.sin_port);   // port_t: 16 bits <-> sin_port: 16 bits

        client->set_ip(client_ip);
        client->set_port(client_port);
        client->set_socket(nclient_sd);

        return client;
    }
    
    int NetConnection::write_packet(char *buffer, size_t num_bytes){
    	
    	packetsize_t packetsize = num_bytes;
    	
    	int bytes_writen = write(sd, &packetsize, sizeof(packetsize_t));
    	if(bytes_writen < 0){
    		perror("(!)Error writing packet");
    		exit(1);
    	}
    	
    	bytes_writen = write(sd, buffer, packetsize);
    	if(bytes_writen < 0){
    		perror("(!)Error writing packet");
    		exit(1);
    	}
    	
    	return bytes_writen;
    }
    
    int NetConnection::write_string(char *buffer){
    	
    	if(CONFIG_WRITE_STRING_SURE){			// To assure that buffer contains a char chain
    		buffer[strlen(buffer)] = '\0';
    	}
    	
    	int bytes_writen = write(sd, buffer, strlen(buffer));
    	if(bytes_writen < 0){
    		perror("(!)Error writing string");
    		exit(1);
    	}
    	
    	return bytes_writen;	
    }
    
	int NetConnection::read_packet(char *buffer, size_t num_bytes){
    	
    	packetsize_t packetsize;
    	
    	int bytes_read = read(sd, &packetsize, sizeof(packetsize_t));
    	if(bytes_read < 0){
    		perror("(!)Error reading packet");
    		exit(1);
    	}else if(bytes_read == 0){
    		printf("(!)Warning: client disconected\n");
    		sleep(5);
    		return 0;
    	}
    	
    	bytes_read = read(sd, buffer, packetsize);
    	if(bytes_read < 0){
    		perror("(!)Error reading packet");
    		exit(1);
    	}else if(bytes_read == 0){
    		printf("(!)Warning: client disconected\n");
    		sleep(5);
    		return 0;
    	}
    	
    	return bytes_read;
    }
    

	void NetConnection::socket_UDP(){
        sd=socket(PF_INET, SOCK_DGRAM, 0);
        if(sd < 0){
            perror("(!)Error setting UDP socket");
            exit(1);
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = ntohs(port);
        server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

        if (bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("(!)Error binding socket");
            exit(1);
        }

    }

    void NetConnection::send_frame(void *msg, size_t size){

    }

    void NetConnection::receive_frame(void *msg, size_t &size){

    }

    void NetConnection::close_connection(){ 
        close(sd);
    }
