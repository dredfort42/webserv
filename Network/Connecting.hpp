//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_CONNECTING_HPP
#define WEBSERV_CONNECTING_HPP

# include "Socket.hpp"

namespace ws
{

	class Connecting: public Socket
	{
	public:
		Connecting(int domain,
				   int service,
				   int protocol,
				   int port,
				   u_long ip);

		int openNewConnection(int socket, struct sockaddr_in address);
	};

} // ws

#endif //WEBSERV_CONNECTING_HPP
