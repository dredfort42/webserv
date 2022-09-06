//
// Created by Dmitry Novikov on 05.09.2022.
//

#include "Client.hpp"

ws::Client::Client(int clientSocket, int maxBodySize)
{
	_clientSocket = clientSocket;
	_bufferSize = 128;
	_maxBodySize = maxBodySize;
	_bytesReceived = 0;
	_bytesSent = 0;
	_request = "";
	_response = "";
}

int 		ws::Client::getClientSocket()
{ return _clientSocket; }

size_t 		ws::Client::getBufferSize()
{ return _bufferSize; }

size_t 		ws::Client::getBytesReceived()
{ return _bytesReceived; }

size_t 		ws::Client::getBytesSent()
{ return _bytesSent; }

std::string ws::Client::getRequest()
{ return _request; }

std::string ws::Client::getResponse()
{ return _response; }

void  		ws::Client::setBytesReceived(size_t bytesReceived)
{ _bytesReceived = bytesReceived; }

void 		ws::Client::setBytesSent(size_t bytesSent)
{ _bytesSent += bytesSent; }

void		ws::Client::setRequest(char *buffer)
{ _request.append(buffer); }

void 		ws::Client::setResponse(std::string response)
{ _response = response; }
