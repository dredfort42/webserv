//
// Created by Dmitry Novikov on 08.08.2022.
//

#include "Socket.hpp"

namespace ws
{
	Socket::Socket(int domain, int service, int protocol, int port, u_long ip)
	{
		// sockaddr_in structure defining
		_address.sin_family = domain;
		_address.sin_port = htons(port);
		_address.sin_addr.s_addr = htonl(ip);

		// Establish socket
		_socket = socket(domain, service, protocol);
		connectionStatus(_socket);

		// Establish network connection
		_connection = openNewConnection(_socket, _address);
		connectionStatus(_connection);
	}

	Socket::~Socket()
	{}

	void Socket::connectionStatus(int item)
	{
		if (connection < 0)
		{
			perror("ERROR: connection failed!");
			exit(EXIT_FAILURE);
		}
	}

	struct sockaddr_in Socket::getAddress()
	{ return _address; }

	int Socket::getSocket()
	{ return _socket; }

	int Socket::getConnection()
	{ return _connection; }

} // ws
