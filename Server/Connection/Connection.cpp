//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Connection.hpp"

namespace ws
{
	Connection::Connection(int socket)
	{
		this->socket = socket;
		request = "";
		response = "";
		bytesSent = 0;
		startActionTime = std::clock();
		lastActionTime = std::clock();
		isReadyToClose = false;
	}
} // ws
