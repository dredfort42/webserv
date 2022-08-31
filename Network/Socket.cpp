//
// Created by Dmitry Novikov on 08.08.2022.
//

#include "Socket.hpp"

ws::Socket::Socket(int domain, int service, int protocol, int port, u_long
ip)
{
	// Address structure defining
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(ip);

	// Establish socket
	_socket = socket(domain, service, protocol);
	socklen_t socklen = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &socklen, sizeof(socklen));
	connectionStatus(_socket);
}

//exception
void ws::Socket::connectionStatus(int item)
{
	if (item < 0)
	{
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in ws::Socket::getAddress()
{ return _address; }

int ws::Socket::getSocket()
{ return _socket; }

int ws::Socket::getConnection()
{ return _connection; }

void ws::Socket::setConnection(int connection)
{
	_connection = connection;
}
