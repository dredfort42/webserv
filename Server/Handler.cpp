//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::handler(Connection &connection)
	{
		time(&connection.startTime);
		char buffer[WS_BUFFER_SIZE + 1];
		memset(&buffer, 0, sizeof(buffer));
		size_t received = recv(connection.socket, buffer, WS_BUFFER_SIZE, 0);
		connection.request.append(buffer);

		// Read complete
		if (received < WS_BUFFER_SIZE) // BAD CONDITION
		{
			FD_CLR(connection.socket, &_masterReadSet);

			std::cout << "\033[34m[HANDLER]\033[0m Socket: ";
			std::cout << connection.socket << std::endl;

			std::cout << "\033[35m";
			std::cout << connection.request;
			std::cout << "\033[0m" << std::endl;

			if (!connection.request.empty())
				processor(connection);
		}
	}

} // ws
