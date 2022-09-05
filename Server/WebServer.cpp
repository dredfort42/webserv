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
		if (service.getServiceStatus())
		{
			_servicesPool.push_front(service);
			int socket = service.getServiceListeningSocket();
			FD_SET(socket, &_masterFdSet);
			if (socket >= _maxFdInMasterSet)
				_maxFdInMasterSet = socket + 1;
		}
	}
	if (!_servicesPool.empty())
		startWebServer();
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
			ws::Acceptor(_servicesPool, _clientsPool,
						 _masterFdSet, _maxFdInMasterSet, _readFdSet);
			for (std::list<Client>::iterator it = _clientsPool.begin();
				 it != _clientsPool.end();
				 it++)
			{
				int clientSocket = it->getClientSocket();

				if (FD_ISSET(clientSocket, &_readFdSet))
				{
					ws::Handler request(*it);
					if (it->getBytesReceived() <= 0)
					{
						removeFromMasterFdSet(clientSocket);
						it = _clientsPool.erase(it);
						continue;
					}
				}

//				if (FD_ISSET(clientSocket, &_writeFdSet))
//				{
//					ws::Responder response(*it);
//					if (it->getResponse().length() - it->getBytesSent() <= 0)
//					{
//						removeFromMasterFdSet(clientSocket);
//						it = _clientsPool.erase(it);
//						continue;
//					}
//				}
			}
		}
	}
}


//void ws::WebServer::responder()
//{
//
//
//
//}

//bool ws::WebServer::sendData(Client client)
//{
//	std::cout << client._response << std::endl;
//	int sendBytes;
//	std::string responsePart;
//
//	sendBytes = send(
//			client._clientSocket,
////			client._response.c_str() + client._bytesAlreadySent,
////			client._responseLength - client._bytesAlreadySent,
//			client._response.c_str(),
//			client._responseLength,
//			0);
//	client._bytesAlreadySent += sendBytes;
//	if (client._bytesAlreadySent >= client._responseLength)
//	{
//		client._bytesAlreadySent = 0;
//		return true;
//	}
//	return false;
//}

//void ws::WebServer::addToMasterFdSet(int socket)
//{
//	FD_SET(socket, &_masterFdSet);
//	if (socket >= _maxFdInMasterSet)
//		_maxFdInMasterSet = socket + 1;
//}

void ws::WebServer::removeFromMasterFdSet(int clientSocket)
{
	FD_CLR(clientSocket, &_masterFdSet);
	close(clientSocket);
	std::cout << "\033[1;31m";
	std::cout << "Close socket: " << clientSocket;
	std::cout << "\033[0m" << std::endl;
}
