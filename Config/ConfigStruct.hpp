
#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace ws {

	struct Location {
		std::string raw;
		std::string 				path;
		std::string					root;
		std::string					index;
		bool						autoindex;
		std::string 				errorPage;
		std::string 				redirect;
		std::string					method;
		std::string					uploadPath;
		std::string					binPath;
	};

	struct Config {
		std::string					ip;
		std::string					port;
		std::string 				serverName;
		bool						autoindex;
		std::string					root;
		std::string					method;
		std::string					index;
		std::string					uploadPath;

		int							bodySize;
		std::string 				errorPage;
		std::vector<ws::Location>	Locations;
	};
}

std::ostream&	operator<<(std::ostream &o, ws::Config const &cnf);
