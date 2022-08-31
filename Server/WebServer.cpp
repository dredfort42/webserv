//
// Created by Dmitry Novikov on 09.08.2022.
//

#include <unistd.h>
#include "WebServer.hpp"
#include "../Config/Parser.hpp"

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
	char message[20] = "[message received]\n";
	write(_newSocket, message, sizeof(message));
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
	ws::Parser parseConf(argv[1]);
	try {
		parseConf.openFile();
		parseConf.readFile();
		parseConf.parseFile();	
	}
	catch (const std::exception& e){
		std::cerr << e.what();
		return (2);
	}
	std::cout << parseConf.getPath() << "\n";
	std::cout << parseConf.getRawText() << "\n";
	ws::WebServer webServer; 
}
