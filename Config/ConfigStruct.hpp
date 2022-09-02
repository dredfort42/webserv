#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

namespace ws {

	struct Location {
		std::string _path;
	};

	struct Config {
		std::string		IP;
		std::string		PORT;
		std::string serverName;
		int			bodySize;
		std::string errorPage;
		std::vector<ws::Location> Locations;
	};
}
