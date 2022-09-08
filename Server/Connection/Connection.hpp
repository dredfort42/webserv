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
		size_t			bytesSent;
		std::clock_t 	startActionTime;
		std::clock_t 	lastActionTime;
		bool 			isReadyToClose;
		HTTPreq			HTTPreq;
	};

} // ws
