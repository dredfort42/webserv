//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::acceptor(fd_set &readSet, int &fdCount)
	{
		for (servIt it = _servicePool.begin();
			 it != _servicePool.end() && fdCount;
			 it++)
		{
			int listeningSocket = it->getListeningSocket();
			if (FD_ISSET(listeningSocket, &readSet))
			{
				FD_CLR(listeningSocket, &readSet);
				fdCount--;

				struct sockaddr address;
				socklen_t addressLen = sizeof(address);
				int socket = accept(listeningSocket, &address, &addressLen);
				if (socket > -1)
				{
					fcntl(socket, F_SETFL, O_NONBLOCK);
					FD_SET(socket, &_masterReadSet);
					if (socket >= _maxFd)
						_maxFd = socket + 1;
					_connectionPool.push_front(Connection(socket, it->getConfig()));

					std::cout << "\033[32m[OPEN]\033[0m Socket: ";
					std::cout << socket << std::endl;
				}
			}
		}
	}

} // ws
