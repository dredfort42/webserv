//
// Created by Dmitry Novikov on 09.08.2022.
//
#pragma once
#define WEBSERV_WEBSERVER_HPP

# include <list>
# include <sys/select.h>
# include <unistd.h>

# include "Service.hpp"
//# include "../Config/ConfigStruct.hpp"

namespace ws
{
	struct Client
	{
		int	_clientSocket;
		int _bodySize;

		Client(int clientSocket, int bodySize):
			_clientSocket(clientSocket), _bodySize(bodySize) {};
	};

	class WebServer
	{
	private:
		int						_maxFdInMasterSet;
		fd_set					_masterFdSet;
		fd_set					_readFdSet;
		fd_set					_writeFdSet;
		std::list<Service>		_servicesPool;
		std::list<Client>		_clientsPool;

		WebServer();

		void	acceptor();
		void	handler();
		void	responder();
		int		openConnection(int listeningSocket);
		bool	receiveData(Client client);
		bool	sendData(Client client, std::string respond);
		void	addToMasterFdSet(int socket);
		void	removeFromMasterFdSet(int socket);

	public:
		WebServer(std::vector<ws::Config> conf);

		void	startWebServer();
	};

} // ws

