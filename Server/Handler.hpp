//
// Created by Dmitry Novikov on 05.09.2022.
//

#ifndef WEBSERV_HANDLER_HPP
#define WEBSERV_HANDLER_HPP

# include <sys/socket.h>
# include "Client.hpp"

namespace ws
{

	class Handler
	{
	private:
		Handler();

	public:
		Handler(Client &client);

	};

} // ws

#endif //WEBSERV_HANDLER_HPP
