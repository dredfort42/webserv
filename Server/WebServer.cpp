//
// Created by Dmitry Novikov on 09.08.2022.
//

#include "WebServer.hpp"

ws::WebServer::WebServer(std::vector<ws::Config> conf)
{
	_maxFdInMasterSet = 0;
	FD_ZERO(&_masterFdSet);
	FD_ZERO(&_readFdSet);
	FD_ZERO(&_writeFdSet);

	// Start services with unique ip:port
	for (std::vector<ws::Config>::iterator it = conf.begin();
		it != conf.end();
		it++)
	{
		ws::Service service(*it);
		_servicesPool.push_front(service);
		addToMasterFdSet(service.getServiceListeningSocket());
	}
}

void ws::WebServer::startWebServer()
{
	// Start web server
	while (true)
	{
		_readFdSet = _masterFdSet;
		_writeFdSet = _masterFdSet;

		if (select(_maxFdInMasterSet, &_readFdSet, &_writeFdSet, NULL, 0) >= 0)
		{
			acceptor();
			handler();
			responder();
		}
	}
}

void ws::WebServer::acceptor()
{
	for (std::list<Service>::iterator it = _servicesPool.begin();
		 it != _servicesPool.end();
		 it++)
	{
		if (FD_ISSET(it->getServiceListeningSocket(), &_readFdSet))
			openConnection(it->getServiceListeningSocket());
	}
}

void ws::WebServer::openConnection(int listeningSocket)
{
	struct sockaddr		address;
	socklen_t 			addressLen;
	int 				clientSocket;

	addressLen = sizeof(address);
	clientSocket = accept(listeningSocket, &address, &addressLen);
	if (clientSocket != -1)
	{
		fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		addToMasterFdSet(clientSocket);
		_clientsPool.push_front(clientSocket);
	}

	std::cout << "Listening socket: " << listeningSocket;
	std::cout << " | New connection socket: " << clientSocket << std::endl;
}

void ws::WebServer::handler()
{
	for (std::list<int>::iterator it = _clientsPool.begin();
		it != _clientsPool.end();
		it++)
	{
		if (FD_ISSET(*it, &_readFdSet) && !receiveData(*it))
		{
			removeFromMasterFdSet(*it);
			it = _clientsPool.erase(it);
			continue;
		}
	}
}

bool ws::WebServer::receiveData(int clientSocket)
{
//	removeFromMasterFdSet(clientSocket);

	char buffer[WS_BUFF_SIZE];
	int receivedBytes;

	memset(buffer, 0, sizeof(buffer));
	receivedBytes = recv(clientSocket, buffer, WS_BUFF_SIZE, 0);
	if (receivedBytes <= 0)
		return false;

	std::cout << "\033[33m" << buffer << "\033[0m" << std::endl;

	return true;
}

void ws::WebServer::responder()
{
	for (std::list<int>::iterator it = _clientsPool.begin();
		 it != _clientsPool.end();
		 it++)
	{
		std::string message;
		message.append("Hello client on socket: ");
		message.append(std::to_string(*it));

		std::string respond;
		respond.append("HTTP/1.1 200 OK\n");
		respond.append("Content-Type: text/plain\n");
		respond.append("Content-Length: ");
		respond.append(std::to_string(message.length()));
		respond.append("\n\n");
		respond.append(message);

		if (FD_ISSET(*it, &_readFdSet) && !sendData(*it, respond))
		{
			removeFromMasterFdSet(*it);
			it = _clientsPool.erase(it);
			continue;
		}
	}
}

void ws::WebServer::addToMasterFdSet(int socket)
{
	FD_SET(socket, &_masterFdSet);
	if (socket >= _maxFdInMasterSet)
		_maxFdInMasterSet = socket + 1;
}

void ws::WebServer::removeFromMasterFdSet(int clientSocket)
{
	FD_CLR(clientSocket, &_masterFdSet);
	close(clientSocket);
	std::cout << "\033[1;31m";
	std::cout << "Close socket: " << clientSocket;
	std::cout << "\033[0m" << std::endl;
}

bool ws::WebServer::sendData(int clientSocket, std::string respond)
{
	std::cout << respond << std::endl;

	char buffer[respond.length()];
	for (int i = 0; respond[i]; i++)
		buffer[i] = respond[i];
	int sendBytes;

	sendBytes = send(clientSocket, buffer, sizeof(buffer), 0);
	if (sendBytes - sizeof(buffer) > 0)
		return true;

	return false;
}
