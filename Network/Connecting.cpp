//
// Created by Dmitry Novikov on 09.08.2022.
//

#include "Connecting.hpp"

ws::Connecting::Connecting(int domain,
						   int service,
						   int protocol,
						   int port,
						   u_long ip) :
	ws::Socket(domain, service, protocol, port, ip)
{
	// Establish network connection
	setConnection(openNewConnection(getListeningSocket(), getAddress()));
	connectionStatus(getConnection());
}

int ws::Connecting::openNewConnection(int socket, struct sockaddr_in address)
{
	return connect(socket, (struct sockaddr *)&address, sizeof(address));
}
