#include "HTTPResponse.hpp"

ws::HTTPResponse::HTTPResponse() {};
ws::HTTPResponse::~HTTPResponse() {};

std::string	ws::HTTPResponse::comparePaths(HTTPreq &req, Config &cnf) {

	std::string reqPath = (cnf.root + req.path);
	(void)cnf;
	std::cout << reqPath <<" PATH\n";
	return reqPath;
}
std::string	ws::HTTPResponse::load(HTTPreq &req, Config &cnf) {
	std::string response, path;

	if (req.method == EMPTY)
		response += "HTTP/1.1 400 Bad Request\n";
	else 
		response += "HTTP/1.1 200 OK\n";

	path = comparePaths(req, cnf);
	return response;
};
