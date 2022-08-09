//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

# include "Server.hpp"

# define WS_BUFF_SIZE 1024
# define WS_DOMAIN AF_INET
# define WS_SERVICE SOCK_STREAM
# define WS_PROTOCOL 0
# define WS_PORT 8080
# define WS_IP INADDR_ANY
# define WS_BACKLOG 100

namespace ws
{

	class WebServer: public Server
	{
	private:
		char	_buffer[WS_BUFF_SIZE];
		int		_newSocket;

		void _accepter();
		void _handler();
		void _responder();

	public:
		WebServer();

		void launcher();
	};

} // ws

#endif //WEBSERV_WEBSERVER_HPP
