//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Server.hpp"

namespace ws
{
	Server::Server(std::vector<Config> config)
	{
		_maxFd = 0;
		FD_ZERO(&_masterReadSet);
		FD_ZERO(&_masterWriteSet);
		_servicePool.clear();
		for (confIt it = config.begin(); it != config.end(); it++)
		{
			Service service = Service(*it);
			if (service.getRunningStatus())
			{
				_servicePool.push_front(service);
				FD_SET(_servicePool.front().getListeningSocket(),
					   &_masterReadSet);
				if (_servicePool.front().getListeningSocket() >= _maxFd)
					_maxFd = _servicePool.front().getListeningSocket() + 1;
			}
		}
	}

	void Server::run()
	{
		struct timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		fd_set readSet;
		fd_set writeSet;
		int fdCount; 

		while (!_servicePool.empty())
		{
			FD_COPY(&_masterReadSet, &readSet);
			FD_COPY(&_masterWriteSet, &writeSet);

			fdCount = select(_maxFd, &readSet, &writeSet, NULL,
								 &timeout);
			if (fdCount)
			{
				std::cout << "\033[36m[SERVER IS RUNNING]\033[0m" << std::endl;
				acceptor(readSet, fdCount);
				for (connIt it = _connectionPool.begin();
					 it != _connectionPool.end() && fdCount;
					 it++)
				{
					if (FD_ISSET(it->socket, &readSet))
					{
						FD_CLR(it->socket, &readSet);
						fdCount--;
						handler(*it);
						continue;
					}

					if (FD_ISSET(it->socket, &writeSet))
					{
						FD_CLR(it->socket, &writeSet);
						fdCount--;
						responder(*it);
						continue;
					}
				}
			} else
				std::cout << "\033[36m[SERVER IS IDLE]\033[0m" << std::endl;
			terminator();
		}
	}
} // ws
