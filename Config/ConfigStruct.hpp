#pragma once
#include <string>

namespace ws {

	struct Location {
		std::string _path;
	};

	struct Config {
		std::string IP;
		std::string PORT;
		std::string serverName;
		int			bodySize;
		std::string errorPage;
		//std::vector<Location> Locations;
	};
}
