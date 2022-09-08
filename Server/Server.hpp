//
// Created by Dmitry Novikov on 07.09.2022.
//

#pragma once

#include <vector>
#include <list>
#include <sys/select.h>
#include <unistd.h>

#include "../Config/ConfigStruct.hpp"
#include "Service/Service.hpp"
#include "Connection/Connection.hpp"

namespace ws
{

	class Server
	{
		typedef std::vector<Config>::iterator confIt;
		typedef std::list<Service>::iterator servIt;
		typedef std::list<Connection>::iterator connIt;

	private:
		int _maxFd;
		fd_set _masterReadSet;
		fd_set _masterWriteSet;
		std::list<Service> _servicePool;
		std::list<Connection> _connectionPool;

		Server();

//


		void acceptor(fd_set &readSet, int &fdCount);
		void	handler(Connection &connection);
		void	processor(Connection &connection);
		void	responder(Connection &connection);


//		int 	openConnection(int listeningSocket);
//

	public:
		Server(std::vector<Config> config);

		void run();
	};

} // ws
