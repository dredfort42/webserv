//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::responder(Connection &connection)
	{
		time(&connection.startTime);

		size_t responseLength = connection.response.length();
		size_t bytesToSend;

		if (responseLength < connection.bytesSent + WS_BUFFER_SIZE)
			bytesToSend = responseLength - connection.bytesSent;
		else
			bytesToSend = WS_BUFFER_SIZE;

		connection.bytesSent += send(
				connection.socket,
				&connection.response[0] + int(connection.bytesSent),
				bytesToSend,
				0
		);

		// Sent complete
		if (responseLength <= connection.bytesSent)
		{
//			std::cout << connection.response << std::endl;

			if (connection.HTTPreq.connect == KEEP_ALIVE)
			{
				FD_CLR(connection.socket, &_masterWriteSet);
				FD_SET(connection.socket, &_masterReadSet);
				connection.request.clear();
				connection.response.clear();
				connection.bytesSent = 0;
			} else
				connection._isReadyToClose = true;
		}
	}

} // ws
