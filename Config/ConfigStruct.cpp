#include "ConfigStruct.hpp"
#include <iomanip>

std::ostream&	operator<<(std::ostream &o, ws::Config const &cnf) {
		o << "----------------------------------------" << std::endl;
		std::cout << "\033[1;32m";
		std::cout << "CONFIG BLOCK : " << cnf.serverName;
		std::cout << "\033[0m" << std::endl;
		o << "----------------------------------------" << std::endl;
		o << std::setw(20) << cnf.ip << " | IP" << std::endl;
		o << std::setw(20) << cnf.port << " | PORT" << std::endl;
		o << std::setw(20) << cnf.serverName << " | server_name\n";
		o << std::setw(20) <<cnf.autoindex << " | autoindex\n";
		o << std::setw(20) <<cnf.root << " | root\n";
		o << std::setw(20) <<cnf.method << " | method\n";
		o << std::setw(20) <<cnf.index << " | index\n";
		o << std::setw(20) <<cnf.uploadPath << " | uploadPath\n";
		o << std::setw(20) <<cnf.bodySize << " | bodySize\n";
	//	o << std::setw(20) <<  cnf.errorPage << " | errorPage\n";

		if (cnf.Locations.empty() == false) 
		{
			o << "----------------------------------------" << std::endl;
			std::cout << "\033[3;35m";
			std::cout << "LOCATIONS";
			std::cout << "\033[0m" << std::endl;
			for (std::vector<ws::Location>::const_iterator it = cnf.Locations.begin(); it != cnf.Locations.end(); it++)
			{
				o << std::setw(20) <<it->path << " | path\n";
				o << std::setw(20) <<it->autoindex << " | autoindex\n";
				o << std::setw(20) <<it->root << " | root\n";
				o << std::setw(20) <<it->method << " | method\n";
				o << std::setw(20) <<it->index << " | index\n";
				o << std::setw(20) <<it->uploadPath << " | uploadPath\n";
				o << std::setw(20) <<it->binPath << " | binPath\n";
		//		o << std::setw(20) <<it->errorPage << " | errorPage\n";
				o << std::setw(20) <<it->redirect << " | redirect\n";
				o << std::setw(20) <<it->raw << " | raw location\n";
				o << "----------------------------------------" << std::endl;
			}
		}
		return o;
	} 

