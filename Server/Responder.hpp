//
// Created by Dmitry Novikov on 05.09.2022.
//

#ifndef WEBSERV_RESPONDER_HPP
#define WEBSERV_RESPONDER_HPP

# include <sys/socket.h>
# include "Client.hpp"


namespace ws
{

	class Responder
	{
	private:
		Responder();

	public:
		Responder(Client client);

	};

} // ws

#endif //WEBSERV_RESPONDER_HPP
