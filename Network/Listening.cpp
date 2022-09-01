//
// Created by Dmitry Novikov on 09.08.2022.
//

#include "Listening.hpp"

ws::Listening::Listening(int domain, int service, int protocol,
						 int port, u_long ip, int backlog) :
	Binding(domain, service, protocol, port, ip)
{
	_backlog = backlog;
	startListening();
	connectionStatus(_listening);
}

void ws::Listening::startListening()
{
	_listening = listen(getListeningSocket(), _backlog);
}
