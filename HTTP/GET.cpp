#include "HTTPResponse.hpp"
#include "../Server/CGI/CGI.hpp"

namespace ws{

	std::string HTTPResponse::GET(ws::HTTPreq &req, ws::Connection &connection,
			ws::Location *loc)
	{
		std::string response;
		if (loc && loc->binPath.empty() == false)
		{
			CGI cgi(req.path, loc, connection);
			response = cgi.getResponse();
			std::string code = cgi.getCode();
			if (code != "200")
				return errorPage(code, connection.setConfig, loc, req);
			return addHeader(response, req, "200");
		} 
		else
		{
			response = responseFromRoot(req, connection.setConfig, loc);
			return response;
		}
	}


	std::string	HTTPResponse::responseFromRoot(HTTPreq &req, resultConfig &cnf, Location *loc)
	{
		if (cnf.method.find(req.method) == std::string::npos)
			return errorPage("405", cnf, loc, req);
		else
		{
			std::cout << req.path << " REQ PATH\n";
			std::string err = "200";
			if (((loc && loc->autoindex && endWith(req.path, loc->path)) || (cnf.autoindex && req.path == "/")))
			{
				if (loc)
				{
					std::cout << endWith(req.path, loc->path) << " BOOL PATH\n";
					req.path = "/" + loc->root + loc->index;
					std::cout << "LOC\n";
				}
				else
					req.path = "/" + cnf.index;
				err = "308";
			}
			else if (loc)
			{
				std::cout << "SUBSTR " << loc->path.size() << " " << req.path.size() << "\n";
				req.path = req.path.substr(loc->path.size(), req.path.size());
			}
			std::cout << req.path << "REQ PATH\n";
			std::cout << cnf.root + req.path << " PATH\n";
			ws::File myFd(cnf.root + req.path, OPEN_FILE);
			if (myFd._fd < 0)
				return errorPage("404", cnf, loc, req);
			else
			{
				std::vector<uint8_t> vect = myFd.readFile();// Мега костыль
				std::string msg(vect.begin(), vect.end());
				std::cout << "HELLO 200\n";
				if (msg.empty())
					return errorPage("404", cnf, loc, req);

				return addHeader(msg, req, "200");
			}
		}
	}
bool	ws::HTTPResponse::endWith(std::string &str, std::string &end)
{
	std::string::reverse_iterator s, e;
	s = str.rbegin();
	e = end.rbegin();
	while (e != end.rend() || s != str.rend())
	{
		if (*s != *e)
			return false;
		s++;
		e++;
	}
	if (e != end.rend())
		return false;
	return true;
}
};
