//
// Created by Dmitry Novikov on 05.09.2022.
//

#ifndef WEBSERV_ACCEPTOR_HPP
#define WEBSERV_ACCEPTOR_HPP

# include <list>

# include "Service.hpp"
# include "Client.hpp"
# include "WebServer.hpp"


namespace ws
{
	class Acceptor
	{
	private:
		Acceptor();

		int openConnection(int listeningSocket);

	public:
		Acceptor(std::list<Service> &servicesPool,
				 std::list<Client> &clientsPool,
				 fd_set &masterFdSet,
				 int &maxFdInMasterSet,
				 fd_set &readFdSet);

	};
} // ws

#endif //WEBSERV_ACCEPTOR_HPP
