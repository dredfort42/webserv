#include "ConfigStruct.hpp"

std::ostream&	operator<<(std::ostream &o, ws::Config const &cnf) {
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
	} 

