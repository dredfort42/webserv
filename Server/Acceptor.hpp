//
// Created by Dmitry Novikov on 05.09.2022.
//

#pragma once

#include <list>

#include "Service.hpp"
#include "Client.hpp"
#include "WebServer.hpp"

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
				 int &maxFdInMasterSet,
				 fd_set &masterFdSet);

	};
} // ws
