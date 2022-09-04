//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

# include <list>

# include "Service.hpp"
# include "../Config/ConfigStruct.hpp"

# define WS_BUFF_SIZE 1024

namespace ws
{

	class WebServer
	{
	private:
		int				_maxFdInMasterSet;
		fd_set			_masterFdSet;
		fd_set			_readFdSet;
		fd_set			_writeFdSet;

		std::list<Service>	_servicesPool;

		WebServer();



//		int					_maxClientSocket;
//		fd_set				_serviceSockets;
//		char				_buffer[WS_BUFF_SIZE];

//		char						_buffer[WS_BUFF_SIZE];
//		int							_selectNumerator;
//		std::vector<ws::Service> 	_servicesPool;
//
//		WebServer();
//
//		void _accepter();
//		void _handler();
//		void _responder();
//
	public:
		WebServer(std::vector<ws::Config> conf);

//		static fd_set	_fdSet;
//
//		WebServer(std::vector<ws::Config> config);
//
//
//		void launcher();
	};

} // ws

#endif //WEBSERV_WEBSERVER_HPP
