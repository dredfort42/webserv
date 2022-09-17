#include "ConfigStruct.hpp"
#include <iomanip>

std::ostream&	operator<<(std::ostream &o, ws::Location const &loc)
{

				o << std::setw(20) <<loc.path << " | path\n";
				o << std::setw(20) <<loc.autoindex << " | autoindex\n";
				o << std::setw(20) <<loc.root << " | root\n";
				o << std::setw(20) <<loc.method << " | method\n";
				o << std::setw(20) <<loc.index << " | index\n";
				o << std::setw(20) <<loc.uploadPath << " | uploadPath\n";
				o << std::setw(20) <<loc.binPath << " | binPath\n";
				o << std::setw(20) <<loc.errorPage.size() << " | errSize\n";
				for (std::map<std::string, std::string>::const_iterator it1 = loc.errorPage.begin(); it1 != loc.errorPage.end(); it1++)
				{
					o << std::setw(20) << it1->first << " | " << it1->second << " | errorPage\n";
				}
				o << std::setw(20) <<loc.redirect << " | redirect\n";
				o << std::setw(20) <<loc.raw << " | raw location\n";
				o << "----------------------------------------" << std::endl;
				return o;
}

std::ostream&	operator<<(std::ostream &o, ws::Config const &cnf) {
	for (std::map<std::string, ws::siteConf>::const_iterator it = cnf.setup.begin(); it != cnf.setup.end(); it++)
	{
		o << "----------------------------------------" << std::endl;
		std::cout << "\033[1;32m";
		std::cout << "CONFIG BLOCK : " << it->first;
		std::cout << "\033[0m" << std::endl;
		o << "----------------------------------------" << std::endl;
		o << std::setw(20) << cnf.ip << " | IP" << std::endl;
		o << std::setw(20) << cnf.port << " | PORT" << std::endl;
		o << std::setw(20) << it->second.serverName << " | server_name\n";
		o << std::setw(20) << it->second.autoindex << " | autoindex\n";
		o << std::setw(20) << it->second.root  << " | root\n";
		o << std::setw(20) << it->second.method << " | method\n";
		o << std::setw(20) << it->second.index << " | index\n";
		o << std::setw(20) << it->second.uploadPath << " | uploadPath\n";
		o << std::setw(20) << it->second.bodySize << " | bodySize\n";
		for (std::map<std::string, std::string>::const_iterator it1 = it->second.errorPage.begin(); it1 != it->second.errorPage.end(); it1++)
		{
			o << std::setw(20) << it1->first << " | " << it1->second << " | errorPage\n";
		}


		if (it->second.Locations.empty() == false) 
		{
			o << "----------------------------------------" << std::endl;
			std::cout << "\033[3;35m";
			std::cout << "LOCATIONS";
			std::cout << "\033[0m" << std::endl;
			for (std::vector<ws::Location>::const_iterator it2 = it->second.Locations.begin(); it2 != it->second.Locations.end(); it2++)
				std::cout << *it2;
		}
	}
		return o;
} 

std::ostream&	operator<<(std::ostream &o, ws::resultConfig const &cnf) {
		o << "----------------------------------------" << std::endl;
		std::cout << "\033[1;32m";
		std::cout << "CONFIG BLOCK : " << cnf.serverName;
		std::cout << "\033[0m" << std::endl;
		o << "----------------------------------------" << std::endl;
		o << std::setw(20) << cnf.ip << " | IP" << std::endl;
		o << std::setw(20) << cnf.port << " | PORT" << std::endl;
		o << std::setw(20) << cnf.serverName << " | server_name\n";
		o << std::setw(20) << cnf.autoindex << " | autoindex\n";
		o << std::setw(20) << cnf.root  << " | root\n";
		o << std::setw(20) << cnf.method << " | method\n";
		o << std::setw(20) << cnf.index << " | index\n";
		o << std::setw(20) << cnf.uploadPath << " | uploadPath\n";
		o << std::setw(20) << cnf.bodySize << " | bodySize\n";
		for (std::map<std::string, std::string>::const_iterator it1 = cnf.errorPage.begin(); it1 != cnf.errorPage.end(); it1++)
		{
			o << std::setw(20) << it1->first << " | " << it1->second << " | errorPage\n";
		}


		if (cnf.Locations.empty() == false) 
		{
			o << "----------------------------------------" << std::endl;
			std::cout << "\033[3;35m";
			std::cout << "LOCATIONS";
			std::cout << "\033[0m" << std::endl;
			for (std::vector<ws::Location>::const_iterator it2 = cnf.Locations.begin(); it2 != cnf.Locations.end(); it2++)
				std::cout << *it2;
		}
		return o;
} 
