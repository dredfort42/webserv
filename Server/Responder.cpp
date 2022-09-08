//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	void Server::responder(Connection &connection)
	{
		connection.lastActionTime = std::clock();
		std::cout << connection.response << std::endl;

//		size_t responseLength = connection.response.length();
//		size_t bytesToSend;
//
//		if (responseLength < connection.bytesSent + connection.bufferSize)
//			bytesToSend = responseLength - connection.bytesSent;
//		else
//			bytesToSend = connection.bufferSize;

		connection.bytesSent = send(
				connection.socket,
//				connection.response.c_str() +
//				connection.bytesSent,
//				bytesToSend,
				connection.response.c_str(),
				connection.response.length(),
				0
		);

		// Sent complete
//		if (responseLength <= connection.bytesSent)
//		{
			connection.response.clear();
			connection.isReadyToClose = true;
//		}
	}

} // ws
