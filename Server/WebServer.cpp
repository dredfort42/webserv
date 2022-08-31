//
// Created by Dmitry Novikov on 09.08.2022.
//

#include <unistd.h>
#include "WebServer.hpp"

ws::WebServer::WebServer() :
Server(WS_DOMAIN, WS_SERVICE, WS_PROTOCOL, WS_PORT, WS_IP, WS_BACKLOG)
{ launcher(); }

void ws::WebServer::_accepter()
{
	int					socket = getSocket()->getSocket();
	struct sockaddr_in	address = getSocket()->getAddress();
	int					addressLen = sizeof(address);


	_newSocket = accept(socket,
						(struct sockaddr *)&address,
						(socklen_t *)&addressLen);
	read(_newSocket, _buffer, WS_BUFF_SIZE);
}

void ws::WebServer::_handler()
{ std::cout << _buffer << std::endl; }

void ws::WebServer::_responder()
{
	std::string message = "HTTP/1.1 200 OK\nContent-Type: "
						  "text/plain\nContent-Length: 12\n\nHello world!!!";
	uint strlen = message.length();
	char messageChr[strlen];
	for (int i = 0; message[i]; i++)
	{
		messageChr[i] = message[i];
	}
	write(_newSocket, messageChr, sizeof(messageChr));
	close(_newSocket);
}

void ws::WebServer::launcher()
{
	while (true)
	{
		std::cout << ">>>>>" << std::endl;
		_accepter();
		_handler();
		_responder();
		std::cout << "<<<<<\n" << std::endl;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << argv[0] <<  ": Wrong number of args!\n";
		return (1);
	}
	//ws::Parser parseConf(argv[1]);

	ws::WebServer webServer; 
}
