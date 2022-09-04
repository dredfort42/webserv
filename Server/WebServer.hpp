//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

# include <list>
# include <sys/select.h>
# include <unistd.h>

# include "Service.hpp"
# include "../Config/ConfigStruct.hpp"

# define WS_BUFF_SIZE 1024

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
		std::list<int>		_clientsPool;

		WebServer();

		void 				acceptor();
		void 				handler();
		void				responder();
		void				addToMasterFdSet(int socket);
		void				removeFromMasterFdSet(int socket);
		void				openConnection(int listeningSocket);
		bool				receiveData(int clientSocket);
		bool				sendData(int clientSocket, std::string respond);

	public:
		WebServer(std::vector<ws::Config> conf);

		void				startWebServer();
	};

} // ws

#endif //WEBSERV_WEBSERVER_HPP
