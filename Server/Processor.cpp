//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::processor(Connection &connection)
	{
		connection.lastActionTime = std::clock();

		//// TMP /////////////////////////////////////////////////////////
		std::string message;
		message.append("Response to socket: ");
		message.append(std::to_string(connection.socket));
		message.append("\nHello client\0");

		connection.response.append("HTTP/1.1 200 OK\n");
		connection.response.append("Content-Type: text/plain\n");
		connection.response.append("Content-Length: ");
		connection.response.append(std::to_string(message.length()));
		connection.response.append("\n\n");
		connection.response.append(message);

		//// END /////////////////////////////////////////////////////////

		// Process complete
		connection.request.clear();
		FD_SET(connection.socket, &_masterWriteSet);

		std::cout << "\033[33m[PROCESSOR]\033[0m Socket: ";
		std::cout << connection.socket;
		std::cout << " >>> TIME: ";
		std::cout << std::clock() - connection.lastActionTime << std::endl;
	}

} // ws
