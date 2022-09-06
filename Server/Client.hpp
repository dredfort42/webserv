//
// Created by Dmitry Novikov on 05.09.2022.
//

#pragma once

#include <iostream>

namespace ws
{

	class Client
	{
	private:
		int				_clientSocket;
		size_t 			_bufferSize;
		int 			_maxBodySize;
		size_t 			_bytesReceived;
		size_t			_bytesSent;
		std::string 	_request;
		std::string 	_response;

		Client();

	public:
		Client(int clientSocket, int maxBodySize);

		int			getClientSocket();
		size_t 		getBufferSize();
		size_t 		getBytesReceived();
		size_t 		getBytesSent();
		std::string	getRequest();
		std::string	getResponse();

		void  	setBytesReceived(size_t bytesReceived);
		void 	setBytesSent(size_t bytesSent);
		void	setRequest(char *buffer);
		void 	setResponse(std::string response);

	};

} // ws
