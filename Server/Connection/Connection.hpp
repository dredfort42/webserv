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
		Connection(int socket, Config &config);

		bool			_isReadyToClose;
		int 			socket;
		std::string 	request;
		std::string 	response;
		size_t			bytesSent;
		std::time_t 	startTime;
		HTTPreq			HTTPreq;
		Config			config;
	};

} // ws
