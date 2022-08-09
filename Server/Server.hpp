//
// Created by Dmitry Novikov on 08.08.2022.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

# include "../Network/Listening.hpp"

namespace ws
{

	class Server
	{
	private:
		Listening *_socket;

		virtual void _accepter() = 0;
		virtual void _handler() = 0;
		virtual void _responder() = 0;

	public:
		Server(int domain,
			   int service,
			   int protocol,
			   int port,
			   u_long ip,
			   int backlog);

		virtual void launcher() = 0;

		Listening *getSocket();
	};

} // ws

#endif //WEBSERV_SERVER_HPP
