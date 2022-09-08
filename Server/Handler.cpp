//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::handler(Connection &connection)
	{
		connection.lastActionTime = std::clock();
		size_t bufferSize = connection.bufferSize;
		char buffer[bufferSize + 1];
		memset(&buffer, 0, sizeof(buffer));
		size_t received = recv(connection.socket, buffer, bufferSize, 0);
		connection.request.append(buffer);

		// Read complete
		if (received < bufferSize) // BAD CONDITION
		{
			FD_CLR(connection.socket, &_masterReadSet);

			std::cout << "\033[34m[HANDLER]\033[0m Socket: ";
			std::cout << connection.socket;
			std::cout << " >>> TIME: ";
			std::cout << std::clock() - connection.lastActionTime << std::endl;

			std::cout << "\033[35m";
			std::cout << connection.request;
			std::cout << "\033[0m" << std::endl;

			processor(connection);
		}
	}

} // ws
