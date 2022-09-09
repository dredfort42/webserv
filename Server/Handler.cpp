//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::handler(Connection &connection)
	{
		char buffer[WS_BUFFER_SIZE + 1];
		memset(&buffer, 0, sizeof(buffer));
		size_t received = recv(connection.socket, buffer, WS_BUFFER_SIZE, 0);
		connection.request.append(buffer);

		// Read complete
		if (received < WS_BUFFER_SIZE) // BAD CONDITION
		{
			FD_CLR(connection.socket, &_masterReadSet);

			std::cout << "\033[34m[HANDLER]\033[0m Socket: ";
			std::cout << connection.socket;
			std::cout << " \033[1;34;42m TIME: ";
			std::cout << std::clock() - connection.startActionTime;
			std::cout << " \033[0m" << std::endl;

			std::cout << "\033[35m";
			std::cout << connection.request;
			std::cout << "\033[0m" << std::endl;

			if (!connection.request.empty())
				processor(connection);
		}
	}

} // ws
