#include "HTTPstruct.hpp"
#include <iomanip>

std::ostream&	operator<<(std::ostream &o, ws::HTTPreq const &req) {
		o << "----------------------------------------" << std::endl;
		o << "\033[1;32m";
		o << "REQUEST RECEIVED";
		o << "\033[0m" << std::endl;
		o << "----------------------------------------" << std::endl;

		o << "PROTOCOL    | " << "HTTP/1.1" << std::endl;
		o << "METHOD      | " << req.method << std::endl;
		o << "PATH        | " << req.path << std::endl;
		o << "HOST        | " << req.host << std::endl;
		o << "PORT        | " << req.port << std::endl;
		o << "CONNECTION  | " << req.connect << std::endl;
		o << "ACCEPT      | " << req.accept << std::endl;
		o << "ACCEPT-Lang | " << req.acceptLang << std::endl;
		o << "ACCEPT-Enc  | " << req.acceptEnc << std::endl;
		o << "User_Agent  | " << req.User_Agent << std::endl;
		

		return o;
}
