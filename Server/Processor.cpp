//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::processor(Connection &connection)
	{
		try {
			ws::HTTPparser req(connection.request);
			
			std::cout << req.getRequest();
			connection.HTTPreq = req.getRequest();
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what();
		}
		std::cout << "CONFIG <<<\n";
		std::cout << connection.config;
		std::cout << "<<<<<<<<<<\n";

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
		FD_SET(connection.socket, &_masterWriteSet);

		std::cout << "\033[33m[PROCESSOR]\033[0m Socket: ";
		std::cout << connection.socket;
		std::cout << " \033[1;34;42m TIME: ";
		std::cout << std::clock() - connection.startActionTime;
		std::cout << " \033[0m" << std::endl;
	}

} // ws
