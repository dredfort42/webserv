
#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace ws {

	struct Location {
		std::string raw;
		std::string 									path;
		std::string										root;
		std::string										index;
		bool											autoindex;
		std::map<std::string, std::string> 				errorPage;
		std::map<std::string, std::string>				redirect;
		std::string										method;
		std::string										uploadPath;
		std::string										binPath;
	};

	struct siteConf {
		std::string										serverName;
		bool											autoindex;
		std::string										root;
		std::string										method;
		std::string										index;
		std::string										uploadPath;
		int												bodySize;
		std::map<std::string, std::string> 				errorPage;
		std::vector<ws::Location>						Locations;
	};

	struct Config {
		std::string										ip;
		std::string										port;
		std::map<std::string, siteConf>					setup;
	};

	struct resultConfig {
		std::string										ip;
		std::string										port;
		std::string 									serverName;
		bool											autoindex;
		std::string										root;
		std::string										method;
		std::string										index;
		std::string										uploadPath;
		int												bodySize;
		std::map<std::string, std::string> 				errorPage;
		std::vector<ws::Location>						Locations;
	};
}

std::ostream&	operator<<(std::ostream &o, ws::Config const &cnf);
std::ostream&	operator<<(std::ostream &o, ws::Location const &loc);
std::ostream&	operator<<(std::ostream &o, ws::resultConfig const &cnf);
