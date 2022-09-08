//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::responder(Connection &connection)
	{
		size_t responseLength = connection.response.length();
		size_t bytesToSend;

		if (responseLength < connection.bytesSent + WS_BUFFER_SIZE)
			bytesToSend = responseLength - connection.bytesSent;
		else
			bytesToSend = WS_BUFFER_SIZE;

		connection.bytesSent += send(
				connection.socket,
				connection.response.c_str() + int(connection.bytesSent),
				bytesToSend,
				0
		);

		// Sent complete
		if (responseLength <= connection.bytesSent)
		{
			connection.lastActionTime = std::clock();
			std::cout << connection.response << std::endl;

			connection.response.clear();
			if (connection.HTTPreq.connect != "keep-alive")
			{
				connection.isReadyToClose = true;
				FD_SET(connection.socket, &_masterReadSet);
			}
		}
	}

} // ws
