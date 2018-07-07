#ifndef __NET_CONNECTION_HPP
#define __NET_CONNECTION_HPP

#define CONFIG_WRITE_STRING_SURE 0	 // If 1, when write an string on the socket, the buffer last position will be set to '\0' for security.

#include <stdint.h>
#include <string>

typedef uint16_t packetsize_t;

class NetConnection
{
	public:
		typedef uint32_t ip_t;
		typedef uint16_t port_t;

	private:
		ip_t ip;
		port_t port;
		int sd;	// Socket descriptor

	public:		
		
		/*   Constructor and destructor   */
		NetConnection();
		NetConnection(port_t new_port);
		NetConnection(ip_t new_ip, port_t new_port);
		NetConnection(std::string domain_name, port_t new_port);
		~NetConnection();
		
		/*   General functions   */ 
		void set_socket(int new_sd);
		int get_socket();
		void set_ip(ip_t new_ip);
		ip_t get_ip();
		void set_port(port_t new_port);
		port_t get_port();
		void close_connection();
		
		/*   TCP   */
		void init_TCP_server(uint8_t max_clients);
		void connect_TCP_client();
		NetConnection *accept_TCP_connection();
		
		/*   Write functions   */
		int write_packet(char *buffer, size_t num_bytes);
		int write_string(char *buffer);
		
		/*   Read functions   */
		int read_packet(char *buffer, size_t num_bytes);
		int read_string(char *buffer);
		


		void socket_UDP();
		void set_domain_name(std::string domain_name);
		void send_frame(void *msg, size_t size);
		void receive_frame(void *msg, size_t &size);
		
};

#endif
