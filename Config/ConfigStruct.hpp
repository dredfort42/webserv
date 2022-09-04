#pragma once
#include <string>
#include <vector>

namespace ws {

	struct Location {
		std::string path;
	};

	struct Config {
		std::string					ip;
		std::string					port;
		std::string 				serverName;
		int							bodySize;
		std::string 				errorPage;
		std::vector<ws::Location>	Locations;
	};
}
