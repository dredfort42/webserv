//
// Created by Dmitry Novikov on 08.08.2022.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>

namespace ws
{

	class Socket
	{
	private:
		struct sockaddr_in _address;
		int _listeningSocket;
		int _connection;

	public:
		Socket(int domain,
			   int service,
			   int protocol,
			   int port,
			   u_long ip);

		virtual int openNewConnection(int listeningSocket, struct sockaddr_in address) = 0;

		void connectionStatus(int item);

		struct sockaddr_in getAddress();
		int getListeningSocket();
		int getConnection();

		void setConnection(int connection);
	};

} // ws

#endif //WEBSERV_SOCKET_HPP
