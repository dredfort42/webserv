#include "HTTPResponse.hpp"
#include "../Server/CGI/CGI.hpp"
#include <string>

ws::HTTPResponse::HTTPResponse() {};
ws::HTTPResponse::~HTTPResponse() {};

ws::Location*	ws::HTTPResponse::findLocation(std::string &path, std::vector<ws::Location> &Locs)
{
	std::vector<ws::Location>::iterator it = Locs.begin();
	for (; it != Locs.end(); it++) {
		if (path.find(it->path) != std::string::npos)
			break;
	}
	if (it == Locs.end())
		return 0;
	return (&(*it));
};

std::string ws::HTTPResponse::GET(ws::HTTPreq &req, ws::Connection &connection, ws::Location *loc) {
	std::string response;
	if (loc && (req.path.find(".php") != std::string::npos || req.path.find(".py")
	!= std::string::npos))
	{
		CGI cgi(req.path, connection.socket, loc);
		response = cgi.getResponse();
		return addHeader(response, req, "200");
	}
	else
	{
		response = responseFromRoot(req, connection.config, loc);
   		return response;
	}
}

std::string ws::HTTPResponse::POST(ws::HTTPreq &req, ws::Connection &connection,
								   ws::Location *loc)
{


	std::string path;
	path = std::getenv("PWD");
	if (loc)
		path = loc->root + loc->uploadPath;
	else
	{
		if (connection.config.uploadPath.empty())
		{
			std::cout << "UPLOAD PATH NOT DEFINED";
			return errorPage("500", connection.config, loc, req);
		}
		path = connection.config.root + connection.config.uploadPath;
	}
	path += std::string("test.txt");


	std::cout << "POST#POST#POST#POST#POST#POST#POST#POST#POST" << std::endl;
	std::cout << path << std::endl;
	std::cout << connection.request << std::endl;

	if (connection.uploadFile._fd == -1)
	{
		connection.uploadFile._fd = open(path.c_str(), O_CREAT | O_RDWR |
		O_APPEND);
		connection.uploadFile._fileOperation = WRITE_FILE;
	}
	connection.uploadFile.addToFile(connection.request);

	return std::string();
};

std::string ws::HTTPResponse::DELETE(ws::HTTPreq &req, ws::Connection &connection, ws::Location *loc) {
	std::string response, path;
	path += std::getenv("PWD");
	if (loc)
		path = loc->root + loc->uploadPath;
	else
	{
		if (connection.config.uploadPath.empty())
		{
			std::cout << "UPLOAD PATH NOT DEFINED";
			return errorPage("500", connection.config, loc, req);
		}
		path = connection.config.root + connection.config.uploadPath;
	}
	path += std::string(req.path.substr(req.path.rfind("/") + 1));
	std::cout << path;
	ws::File myFd(path, OPEN_FILE);
	if (myFd._fd < 0)
		return addHeader(response, req, "204");
//	std::vector<uint8_t> tmp = myFd.readFile();
//	response = std::string(tmp.begin(), tmp.end()); 
	myFd.removeFile();
	return addHeader(response, req, "200");

}

std::string	ws::HTTPResponse::load(HTTPreq &req, Connection &connection) {
	std::string response;
	
	ws::Location *loc = findLocation(req.path, connection.config.Locations);

	if (loc)
		std::cout << *loc;

	if (loc && loc->method.find(req.method) == std::string::npos)
		return errorPage("400", connection.config, loc, req);
	else if (connection.config.method.find(req.method) == std::string::npos)
		return errorPage("400", connection.config, loc, req);

	if (req.method == "GET")
		return GET(req, connection, loc);
	else if (req.method == "POST")
		return POST(req, connection, loc);
	else if (req.method == "DELETE")
		return DELETE(req, connection, loc);


	return response;
};

inline std::string& ws::HTTPResponse::trim( std::string &line, const std::string &trimmer)
{
	line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
    line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
	return line;
};

std::string ws::HTTPResponse::errorPage(const std::string &err, ws::Config &cnf, ws::Location *loc, ws::HTTPreq &req) {
	std::vector<uint8_t> response;
	std::string path;
	ws::File myFd;
	if (loc && loc->errorPage[err].empty() == false)
		path = loc->errorPage.at(err);
	else if (cnf.errorPage[err].empty() == false)
		path = cnf.errorPage.at(err);
	else
		return notFound();
	std::cout << path << " ERROR PATH\n";
	myFd.setPath(path, OPEN_FILE);
	if (myFd._fd < 0)
		return notFound();
	response = myFd.readFile();
	std::string msg(response.begin(), response.end());
	return addHeader(msg, req, err);
}

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

std::string ws::HTTPResponse::addHeader(std::string& msg, ws::HTTPreq& req, const std::string& code)
{
	std::string head, extension, accept;

	extension = ws::File::getFileType(req.path);

	head.append("HTTP/1.1 ");
	head.append(code);
	head.append("\n");
	head.append("Content-Type: ");
	while (req.accept.empty() == false)
	{
		accept = Split(req.accept, ",");
		if (accept.find(extension) != std::string::npos)
			break;
	}
	head.append(accept + "\n");
//	if (extension == "png")
//	{
//		head.append("Content-Encoding: gzip\n");
//	}
	head.append("Content-Length: ");
	head.append(std::to_string(msg.size()));
	head.append("\n\n");
	head.append(msg);
	return head;
}
std::string	ws::HTTPResponse::responseFromRoot(HTTPreq &req, Config &cnf, Location *loc)
{
	if (cnf.method.find(req.method) == std::string::npos)
		return errorPage("400", cnf, loc, req);
	else
	{
		if (((loc && loc->autoindex) || (cnf.autoindex)) && *(req.path.rbegin()) == '/')
			req.path += cnf.index;
		ws::File myFd(cnf.root + req.path, OPEN_FILE);	
		if (myFd._fd < 0)
			return errorPage("404", cnf, loc, req);
		else
		{
			std::vector<uint8_t> vect = myFd.readFile();// Мега костыль
			std::string msg(vect.begin(),vect.end());
			
			if (msg.empty())
				return errorPage("404", cnf, loc, req);
			return addHeader(msg, req, "200");
		}
	}
}

std::string	ws::HTTPResponse::Split(std::string &line, std::string delimiter)
{
	size_t pos = 0;
	std::string token;
	pos = line.find(delimiter);
	if (pos == std::string::npos)
	{
		token = line;
		line.erase();
		return (this->trim(token, " \t"));
	}

    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
	line.append("\0");
	return (trim(token, " \t"));
}
