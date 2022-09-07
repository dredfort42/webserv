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
	while (!_servicesPool.empty())
	{
		_readFdSet = _masterFdSet;
		_writeFdSet = _masterFdSet;

		if (select(_maxFdInMasterSet, &_readFdSet, &_writeFdSet, NULL, 0) >= 0)
		{
			ws::Acceptor(_servicesPool, _clientsPool,
						 _maxFdInMasterSet, _masterFdSet);

			for (std::list<Client>::iterator it = _clientsPool.begin();
				 it != _clientsPool.end();
				 it++)
			{
				int clientSocket = it->getClientSocket();

				if (FD_ISSET(clientSocket, &_readFdSet))
				{
					FD_CLR(clientSocket, &_readFdSet);
					ws::Handler request(*it);
					if (it->getBytesReceived() <= 0)
					{
						//std::cout << (*it).getRequest();
						ws::HTTPparser req((*it).getRequest());
						try {
							std::cout << req.getRequest();
						}
						catch (const std::exception &e) {
							std::cerr << e.what();
							removeFromMasterFdSet(clientSocket);
						}
						removeFromMasterFdSet(clientSocket);
						it = _clientsPool.erase(it);
						continue;
					}
				}

				if (FD_ISSET(clientSocket, &_writeFdSet))
				{
					FD_CLR(clientSocket, &_writeFdSet);
				//	ws::Responder response(*it);
				//	if (it->getResponse().length() - it->getBytesSent() <= 0)
				//	{
				//		removeFromMasterFdSet(clientSocket);
				//		it = _clientsPool.erase(it);
				//		continue;
				//	}
				}
			}
		}
	}
}

void ws::WebServer::removeFromMasterFdSet(int clientSocket)
{
	FD_CLR(clientSocket, &_masterFdSet);
	close(clientSocket);
	std::cout << "\033[1;31m";
	std::cout << "Close socket: " << clientSocket;
	std::cout << "\033[0m" << std::endl;
}
