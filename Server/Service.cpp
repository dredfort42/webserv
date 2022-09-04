//
// Created by Dmitry Novikov on 08.08.2022.
//

#include "Service.hpp"

ws::Service::Service(ws::Config serviceConfig)
{
	establishListeningSocket();
	getSockAddr(serviceConfig);
	establishNetworkConnection();
	startListeningSocket();


	// Print service info
	if (_serviceStartedSuccessfully)
	{
		std::cout << "\n----------------------------------------" << std::endl;
		std::cout << "\033[1;32m";
		std::cout << "SERVICE SUCCESSFULLY STARTED";
		std::cout << "\033[0m" << std::endl;
		std::cout << "Address: " << serviceConfig.ip;
		std::cout << ":" << serviceConfig.port << " | ";
		std::cout << "Socket: " << _listeningSocket << std::endl;
		std::cout << "----------------------------------------\n" << std::endl;
	}
}

void	ws::Service::establishListeningSocket()
{
	// Establish socket
	_listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(_listeningSocket, F_SETFL, O_NONBLOCK);
	_serviceStartedSuccessfully = serviceStatus(_listeningSocket,
												 "Establish socket");
	if (_serviceStartedSuccessfully)
	{
		int optionLen = 1;
		_serviceStartedSuccessfully = serviceStatus(
				setsockopt(_listeningSocket,
						   SOL_SOCKET,
						   SO_REUSEADDR,
						   &optionLen,
						   sizeof(optionLen)), "Make socket reusable");
	}
}

void	ws::Service::getSockAddr(ws::Config serviceConfig)
{
	// Address structure defining
	if (_serviceStartedSuccessfully)
	{
		memset(&_address, 0, sizeof(_address));
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = inet_addr(serviceConfig.ip.c_str());
		_address.sin_port = htons(atoi(serviceConfig.port.c_str()));
	}
}

void 	ws::Service::establishNetworkConnection()
{
	// Establish network connection
	if (_serviceStartedSuccessfully)
	{
		_serviceStartedSuccessfully = serviceStatus(
			bind(
				_listeningSocket,
				(struct sockaddr *) &_address,
				sizeof(_address)
			),
			"Establish network connection"
		);
	}
}

void ws::Service::startListeningSocket()
{
	// Start listening socket
	if (_serviceStartedSuccessfully)
	{
		_serviceStartedSuccessfully = serviceStatus(
			listen(_listeningSocket, WS_BACKLOG),
			"Start listening socket"
		);
	}
}

bool ws::Service::serviceStatus(int rtn, std::string step)
{
	if (rtn < 0)
	{
		std::cout << "\033[31m" << "[-] " << step << "\033[0m" << std::endl;
		return false;
	}
	std::cout << "\033[32m" << "[+] " << "\033[0m" << step << std::endl;
	return true;
}

struct sockaddr_in ws::Service::getServiceAddress()
{
	return _address;
}

int ws::Service::getServiceListeningSocket()
{
	return _listeningSocket;
}
