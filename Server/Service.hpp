//
// Created by Dmitry Novikov on 08.08.2022.
//

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "../Config/Parser.hpp"
#include "../HTTP/HTTPparser.hpp"
# define WS_BACKLOG 64

namespace ws
{

	class Service
	{
	private:
		bool 				_serviceStartedSuccessfully;
		int 				_maxBodySize;
		int					_listeningSocket;
		struct sockaddr_in 	_address;

		Service();

		void 				checkServiceStatus(int rtn, std::string step);
		void				establishListeningSocket();
		void 				getSockAddr(ws::Config serviceConfig);
		void 				establishNetworkConnection();
		void 				startListeningSocket();
		void 				printServiceInfo(ws::Config serviceConfig);


	public:
		Service(ws::Config serviceConfig);

		bool 				getServiceStatus();
		int 				getMaxBodySize();
		int 				getServiceListeningSocket();
	};

} // ws
