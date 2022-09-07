#include "HTTPstruct.hpp"
#include <iomanip>

std::ostream&	operator<<(std::ostream &o, ws::HTTPreq const &req) {
		o << "----------------------------------------" << std::endl;
		o << "\033[1;32m";
		o << "REQUEST RECEIVED";
		o << "\033[0m" << std::endl;
		o << "----------------------------------------" << std::endl;

		o << std::setw(20) << req.method << " | METHOD" << std::endl;
		o << std::setw(20) << req.path << " | PATH" << std::endl;
		o << std::setw(20) << "HTTP/1.1" << " | ver.HTTP" << std::endl;

		return o;
}
