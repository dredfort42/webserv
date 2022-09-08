//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{
	void Server::terminator()
	{
		for (connIt it = _connectionPool.begin();
			 it != _connectionPool.end();
			 it++)
		{
			if (it->isReadyToClose
				|| std::clock() - it->lastActionTime > WS_CONNECTION_TIMEOUT)
			{
				std::cout << "\033[31m[CLOSE]\033[0m Socket: ";
				std::cout << it->socket;
				std::cout << " \033[1;34;42m TIME: ";
				std::cout << std::clock() - it->startActionTime;
				std::cout << " \033[0m" << std::endl;

				if (FD_ISSET(it->socket, &_masterReadSet))
					FD_CLR(it->socket, &_masterReadSet);
				if (FD_ISSET(it->socket, &_masterWriteSet))
					FD_CLR(it->socket, &_masterWriteSet);
				close(it->socket);
				it = _connectionPool.erase(it);
			}
		}
	}
} // ws
