//
// Created by Dmitry Novikov on 09.08.2022.
//

#include <unistd.h>
#include <fcntl.h>
#include "WebServer.hpp"
#include "../Config/Parser.hpp"
#include "../Config/ConfigStruct.hpp"

ws::WebServer::WebServer() :
Server(WS_DOMAIN, WS_SERVICE, WS_PROTOCOL, WS_PORT, WS_IP, WS_BACKLOG)
{
	memset(_buffer, 0 ,sizeof(_buffer));
	FD_ZERO(&_fdSet);
	_selectNumerator = 0;
	launcher();
}

void ws::WebServer::_accepter()
{
	int					listeningSocket = getListeningSocket()->getListeningSocket();
	struct sockaddr_in	address = getListeningSocket()->getAddress();
	int					addressLen = sizeof(address);

	_clientSocket = accept(listeningSocket,
						(struct sockaddr *)&address,
						(socklen_t *)&addressLen);
	if (_clientSocket > _selectNumerator)
		_selectNumerator = _clientSocket + 1;
	fcntl(_clientSocket, F_SETFL, O_NONBLOCK);
	read(_clientSocket, _buffer, WS_BUFF_SIZE);
}

void ws::WebServer::_handler()
{ std::cout << _buffer << std::endl; }

void ws::WebServer::_responder()
{
	std::string message = "HTTP/1.1 200 OK\nContent-Type: "
						  "text/plain\nContent-Length: 22\n\nHello "
						  "world 0123456789\n";
	char messageChr[message.length()];
	for (int i = 0; message[i]; i++)
		messageChr[i] = message[i];

	write(_clientSocket, messageChr, sizeof(messageChr));

	close(_clientSocket);
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
	std::vector<ws::Config> tokens;
	try {
		tokens = parseConf.getStruct();
	}
	catch (const std::exception& e){
		std::cerr << e.what();
		return (2);
	}
	ws::WebServer webServer; 
}
