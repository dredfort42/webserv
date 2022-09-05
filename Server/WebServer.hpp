//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

# include <iostream>
# include <list>
# include <sys/select.h>
# include <unistd.h>

# include "Service.hpp"
//# include "Client.hpp"
# include "Acceptor.hpp"
# include "Handler.hpp"
# include "Responder.hpp"
# include "../Config/ConfigStruct.hpp"

namespace ws
{

	class WebServer
	{
	private:
		int					_maxFdInMasterSet;
		fd_set				_masterFdSet;
		fd_set				_readFdSet;
		fd_set				_writeFdSet;
		std::list<Service>	_servicesPool;
		std::list<Client>	_clientsPool;

		WebServer();

	public:
		WebServer(std::vector<ws::Config> conf);

		void	startWebServer();
		void	removeFromMasterFdSet(int socket);
	};

} // ws

#endif //WEBSERV_WEBSERVER_HPP
