//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Server.hpp"

namespace ws
{
	Server::Server(std::vector <Config> config)
	{
		_maxFd = 0;
		FD_ZERO(&_masterReadSet);
		FD_ZERO(&_masterWriteSet);
		_servicePool.clear();
		for (confIt it = config.begin(); it != config.end(); it++)
		{
			_servicePool.push_front(Service(*it));
			FD_SET(_servicePool.front().getListeningSocket(), &_masterReadSet);
			if (_servicePool.front().getListeningSocket() >= _maxFd)
				_maxFd = _servicePool.front().getListeningSocket() + 1;
		}
	}

	void Server::run()
	{
		struct timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		while (_maxFd)
		{

			fd_set readSet;
			fd_set writeSet;
			FD_COPY(&_masterReadSet, &readSet);
			FD_COPY(&_masterWriteSet, &writeSet);

			int fdCount = select(_maxFd, &readSet, &writeSet, NULL, &timeout);
			if (fdCount)
			{
				acceptor(readSet, fdCount);
				for (connIt it = _connectionPool.begin();
					 it != _connectionPool.end() && fdCount;
					 it++)
				{

					std::cout << "TIME: ";
					std::cout << std::clock() - it->lastActionTime << std::endl;

					if (it->isReadyToClose
						|| std::clock() - it->lastActionTime > 5000)
					{
						std::cout << "\033[31m[X]\033[0m Socket: ";
						std::cout << it->socket;
						std::cout << " >>> TIME: ";
						std::cout << std::clock() - it->lastActionTime <<
								  std::endl;

						FD_CLR(it->socket, &_masterWriteSet);
						close(it->socket);
						it = _connectionPool.erase(it);
						std::cout << "\033[31m[CLOSE]\033[0m" << std::endl;
						continue;
					}

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
				std::cout << "\033[33m[IDLE]\033[0m" << std::endl;


			usleep(100);
		}
	}

} // ws
