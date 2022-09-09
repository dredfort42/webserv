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
		lastActionTime = startActionTime;
	}
	
	Connection::Connection(int socket, ws::Config &config)
	{
		this->socket = socket;
		this->config = config;
		request = "";
		response = "";
		bytesSent = 0;
		startActionTime = std::clock();
		lastActionTime = startActionTime;
		isReadyToClose = false;
	}
} // ws
