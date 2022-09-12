#include "HTTPResponse.hpp"
#include "../Server/CGI/CGI.hpp"
#include "../Server/File/File.hpp"
#include <string>

ws::HTTPResponse::HTTPResponse() {};
ws::HTTPResponse::~HTTPResponse() {};

ws::Location*	ws::HTTPResponse::findLocation(std::string &path, std::vector<ws::Location> &Locs)
{
	std::vector<ws::Location>::iterator it = Locs.begin();
	for (; it != Locs.end(); it++) {
		if (path.find(it->path) == 0)
			break;
	}
	if (it == Locs.end())
		return 0;
	return (&(*it));
};

std::string	ws::HTTPResponse::load(HTTPreq &req, Config &cnf) {
	std::string response;
	
	ws::Location *loc = findLocation(req.path, cnf.Locations);
	if (!loc && req.path.rfind("/") != 0)
	{
		response = notFound();
		return response;
	}
	else if (loc && loc->path == "/cgi-bin/")
	{
		//Your Code Дима)))
		//		std::string 	tmpPath = "www/server3/";
		//		std::string  	tmpFileName = "index.php";
		CGI file(req.path);

		// get mime type
		MimeType mime = ws::File::getFileType(req.path);
		std::cout << mime << std::endl;

		response = file.getResponse();
	}
	else
	{
		if (!loc)
			response = responseFromRoot(req, cnf);
	//	else
	//		response = bodyFromLoc(req, loc, cnf);
		return response;
	}
	std::string message;
	message.append("\nHello client\0");
	response.append("HTTP/1.1 200 OK\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: ");
	response.append(std::to_string(message.length()));
	response.append("\n\n");
	response.append(message);
//	response = comparePaths(req, cnf);
	return response;
};

inline std::string& ws::HTTPResponse::trim( std::string &line, const std::string &trimmer)
{
	line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
    line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
	return line;
};

std::string	ws::HTTPResponse::notFound() {
	std::string response, msg;
	response.append("HTTP/1.1 404 Not Found\r\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: 11\n\n");
//	response.append("Content-Length: ");
	response.append("Not Found!\n"); // Добавить чтение их html 

	return (response);
}

std::string	ws::HTTPResponse::badRequest() {
	std::string response, msg;
	response.append("HTTP/1.1 400 Bad request\r\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: 13\n");
//	response.append("Content-Length: ");
	response.append("Bad request!\n"); // Добавить чтение их html 

	return (response);
}

std::string ws::HTTPResponse::addHeader(std::string& msg, std::string& file)
{
	std::string head;

	head.append("HTTP/1.1 200 OK\n");
	head.append("Content-Type: text/");
	file.erase(0, file.rfind(".") + 1);
	head.append(file + "\n");
	head.append("Content-Length: ");
	head.append(std::to_string(msg.size()));
	head.append("\n\n");
	head.append(msg);
	return head;
}
std::string	ws::HTTPResponse::responseFromRoot(HTTPreq &req, Config &cnf)
{
	if (cnf.method.find(req.method) == std::string::npos)
		return badRequest();
	else
	{
		if (req.path == "/")
			req.path += cnf.index;
		ws::File myFd(cnf.root + req.path, OPEN_FILE);	
		if (myFd._fd < 0)
			return notFound();
		else
		{
			std::string msg = myFd.readAll();
			if (msg.empty())
				return notFound();
			return addHeader(msg, req.path);
		}
	}
}

