//
// Created by Dmitry Novikov on 07.09.2022.
//

#pragma once

#include <ctime>
#include "../Service/Service.hpp"

namespace ws
{

	struct Connection
	{
		Connection(int socket);

		int 			socket;
		std::string 	request;
		std::string 	response;
		size_t			bufferSize;
		size_t			bytesSent;
		std::clock_t 	lastActionTime;
		bool 			isReadyToClose;
	};

} // ws
