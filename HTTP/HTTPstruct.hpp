#pragma once
#include <iostream>

namespace ws {
	struct HTTPreq {
		std::string method;
		std::string path;
		std::string host;	
	};

}
std::ostream&	operator<<(std::ostream &o, ws::HTTPreq const &req);
