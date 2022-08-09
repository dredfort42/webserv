//
// Created by Dmitry Novikov on 09.08.2022.
//

#include "Binding.hpp"

ws::Binding::Binding(int domain,
					 int service,
					 int protocol,
					 int port,
					 u_long ip) :
	ws::Socket(domain, service, protocol, port, ip)
{
	// Establish network connection
	setConnection(openNewConnection(getSocket(), getAddress()));
	connectionStatus(getConnection());
}

int ws::Binding::openNewConnection(int socket, struct sockaddr_in address)
{
	return bind(socket, (struct sockaddr *)&address, sizeof(address));
}
