//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_BINDING_HPP
#define WEBSERV_BINDING_HPP

# include "Socket.hpp"

namespace ws
{

	class Binding: public Socket
	{

	public:
		Binding(int domain,
				int service,
				int protocol,
				int port,
				u_long ip);

		int openNewConnection(int socket, struct sockaddr_in address);

	};

} // ws

#endif //WEBSERV_BINDING_HPP
