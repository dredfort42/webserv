#pragma once
#include <string>
#include <vector>
#include <iostream>
namespace ws {

	struct Location {
		std::string path;
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
/*
	std::ostream&	operator<<(std::ostream &o, const Config& cnf) {
		o << cnf.ip << " IP\n";
		o << cnf.port << " PORT\n";
		o << cnf.serverName << " server_name\n";
		o << cnf.autoindex << " autoindex\n";
		o << cnf.root << " root\n";
		o << cnf.method << " method\n";
		o << cnf.index << " index\n";
		o << cnf.uploadPath << " uploadPath\n";
		o << cnf.bodySize << " bodySize\n";
		o << cnf.errorPage << " errorPage\n";
		return o;
	} */
}
