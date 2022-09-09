//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::terminator()
	{
		std::time_t currentTime;

		time(&currentTime);
		for (connIt it = _connectionPool.begin();
			 it != _connectionPool.end();
			 it++)
		{
			if (it->_isReadyToClose
				|| difftime(currentTime, it->startTime) > WS_CONNECTION_TIMEOUT)
			{
				std::cout << "\033[31m[CLOSE]\033[0m Socket: ";
				std::cout << it->socket << std::endl;

				FD_CLR(it->socket, &_masterReadSet);
				FD_CLR(it->socket, &_masterWriteSet);
				close(it->socket);
				it = _connectionPool.erase(it);
			}
		}
	}

} // ws
