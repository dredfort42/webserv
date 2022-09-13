//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Connection.hpp"

namespace ws
{

	Connection::Connection(int socket, ws::Config &config)
	{
		_isReadyToClose = false;
		this->socket = socket;
		this->config = config;
		request = "";
		response = "";
		bytesSent = 0;
		time(&startTime);
		
	}

} // ws
