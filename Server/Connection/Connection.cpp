//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Connection.hpp"

namespace ws
{
	Connection::Connection(int socket)
	{
		this->socket = socket;
		bufferSize = 128;
		request = "";
		response = "";
		lastActionTime = std::clock();
		isReadyToClose = false;
	}
} // ws
