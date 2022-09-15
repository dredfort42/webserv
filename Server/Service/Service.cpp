//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Service.hpp"

namespace ws
{
	Service::Service(Config config)
	{
		_isServiceRunning = true;
		_serviceConfig = config;
		startListeningSocket();
	}

	void Service::startListeningSocket()
	{
		_listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (check(_listeningSocket, "Establish socket"))
		{
			fcntl(_listeningSocket, F_SETFL, O_NONBLOCK);
			makeSocketReusable();
		}
		else
			printServiceConfig();

	}

	void Service::makeSocketReusable()
	{
		int optionLen = 1;
		int stepStatus = setsockopt(
				_listeningSocket,
				SOL_SOCKET,
				SO_REUSEADDR,
				&optionLen,
				sizeof(optionLen)
		);
		if (check(stepStatus, "Make socket reusable"))
			makeSocketSilent();
		else
			printServiceConfig();
	}

	void Service::makeSocketSilent()
	{
		int optionLen = 1;
		int stepStatus = setsockopt(
				_listeningSocket,
				SOL_SOCKET,
				SO_NOSIGPIPE,
				&optionLen,
				sizeof(optionLen)
		);
		if (check(stepStatus, "Make socket silent"))
			prepareNetworkConnection();
		else
			printServiceConfig();
	}

	void Service::prepareNetworkConnection()
	{
		struct sockaddr_in address;
		memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(_serviceConfig.ip.c_str());
		address.sin_port = htons(atoi(_serviceConfig.port.c_str()));
		int stepStatus = bind(
				_listeningSocket,
				(struct sockaddr *) &address,
				sizeof(address)
		);
		if (check(stepStatus, "Prepare network connection"))
			establishNetworkConnection();
		else
			printServiceConfig();
	}

	void Service::establishNetworkConnection()
	{
		int stepStatus = listen(_listeningSocket, WS_BACKLOG);
		check(stepStatus, "Start listening socket");
		printServiceConfig();
	}

	bool Service::check(int stepStatus, std::string stepName)
	{
		if (stepStatus < 0)
		{
			std::cout << "\033[31m" << "[-] ";
			std::cout << stepName << "\033[0m" << std::endl;
			_isServiceRunning = false;
			return (false);
		} else
		{
			std::cout << "\033[32m" << "[+] " << "\033[0m";
			std::cout << stepName << std::endl;
			return (true);
		}
	}

	void Service::printServiceConfig()
	{
		std::cout << "----------------------------------------" << std::endl;
		if (_isServiceRunning)
		{
			std::cout << "\033[1;32m";
			std::cout << "SERVICE SUCCESSFULLY STARTED";
			std::cout << "\033[0m" << std::endl;
			std::cout << "Address: " << _serviceConfig.ip;
			std::cout << ":" << _serviceConfig.port << " | ";
			std::cout << "Socket: " << _listeningSocket << std::endl;
		} else
		{
			std::cout << "\033[1;31m";
			std::cout << "FAILED TO START SERVICE";
			std::cout << "\033[0m" << std::endl;
		}
		std::cout << "----------------------------------------\n" << std::endl;
	}

	int Service::getListeningSocket()
	{
		return _listeningSocket;
	}

	bool Service::getRunningStatus()
	{
		return _isServiceRunning;
	}

	Config& Service::getConfig()
	{
		return _serviceConfig;
	}

} // ws
