//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{
	void Server::terminator(Connection &connection)
	{
		std::cout << "\033[31m[CLOSE]\033[0m Socket: ";
		std::cout << connection.socket;
		std::cout << " \033[1;34;42m TIME: ";
		std::cout << std::clock() - connection.startActionTime;
		std::cout << " \033[0m" << std::endl;

		FD_CLR(connection.socket, &_masterReadSet);
		FD_CLR(connection.socket, &_masterWriteSet);
		close(connection.socket);
	}

	void Server::activityCheck()
	{
		for (connIt it = _connectionPool.begin();
			 it != _connectionPool.end();
			 it++)
		{
			if (std::clock() - it->lastActionTime > WS_CONNECTION_TIMEOUT)
			{
				std::cout << "\033[1;37;41m[TIMEOUT]\033[0m Socket: ";
				std::cout << it->socket;
				std::cout << " \033[1;34;42m TIME: ";
				std::cout << std::clock() - it->startActionTime;
				std::cout << " \033[0m" << std::endl;

				terminator(*it);
				it = _connectionPool.erase(it);
			}
		}
	}

} // ws
