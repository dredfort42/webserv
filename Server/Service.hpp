//
// Created by Dmitry Novikov on 08.08.2022.
//

#ifndef WEBSERV_SERVICE_HPP
#define WEBSERV_SERVICE_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>

# include "../Config/ConfigStruct.hpp"

# define WS_BACKLOG 64

namespace ws
{

	class Service
	{
	private:
		int 				_listeningSocket;
		struct sockaddr_in 	_address;
		bool 				_serviceStartedSuccessfully;

		Service();

		bool 				getServiceStatus(int rtn, std::string step);
		void				establishListeningSocket();
		void 				getSockAddr(ws::Config serviceConfig);
		void 				establishNetworkConnection();
		void 				startListeningSocket();
		void 				printServiceInfo(ws::Config serviceConfig);


	public:
		Service(ws::Config serviceConfig);

		int					getServiceListeningSocket();
	};

} // ws

#endif //WEBSERV_SERVICE_HPP
