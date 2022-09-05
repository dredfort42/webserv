//
// Created by Dmitry Novikov on 05.09.2022.
//

#include "Acceptor.hpp"

ws::Acceptor::Acceptor(std::list<Service> &servicesPool,
					   std::list<Client> &clientsPool,
					   int &maxFdInMasterSet,
					   fd_set &masterFdSet)
{
	for (std::list<Service>::iterator it = servicesPool.begin();
		 it != servicesPool.end();
		 it++)
	{
		if (FD_ISSET(it->getServiceListeningSocket(), &masterFdSet))
		{
			int clientSocket = openConnection(it->getServiceListeningSocket());
			if (clientSocket != -1)
			{
				fcntl(clientSocket, F_SETFL, O_NONBLOCK);
				FD_SET(clientSocket, &masterFdSet);
				if (clientSocket >= maxFdInMasterSet)
					maxFdInMasterSet = clientSocket + 1;
				clientsPool.push_front(Client(clientSocket, it->getMaxBodySize()));

				std::cout << "Listening socket: " << it->getServiceListeningSocket();
				std::cout << " | New connection socket: " << clientSocket << std::endl;
			}
		}
	}
}

int ws::Acceptor::openConnection(int listeningSocket)
{
	struct sockaddr		address;
	socklen_t 			addressLen;
	int 				clientSocket;

	addressLen = sizeof(address);
	clientSocket = accept(listeningSocket, &address, &addressLen);
	if (clientSocket == -1)
		return -1;
	return clientSocket;
}
