//
// Created by Dmitry Novikov on 07.09.2022.
//

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "../../Config/ConfigStruct.hpp"

#define WS_BACKLOG 64
#define WS_BUFFER_SIZE 1024
#define WS_CONNECTION_TIMEOUT 10000

namespace ws
{

	class Service
	{
	private:
		bool		_isServiceRunning;
		ws::Config	_serviceConfig;
		int			_listeningSocket;

		Service();

		bool check(int stepStatus, std::string stepName);
		void startListeningSocket();
		void makeSocketReusable();
		void prepareNetworkConnection();
		void establishNetworkConnection();
		void printServiceConfig();

	public:
		Service(Config config);

		int 	getListeningSocket();
		bool 	getRunningStatus();

	};

} // ws
