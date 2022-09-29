#include "HTTPResponse.hpp"
#include "../Server/CGI/CGI.hpp"
#include <string>

ws::HTTPResponse::HTTPResponse()
{};

ws::HTTPResponse::~HTTPResponse()
{};

ws::Location *ws::HTTPResponse::findLocation(std::string &path,
											 std::vector<ws::Location> &Locs)
{
	std::vector<ws::Location>::iterator it = Locs.begin();
	for (; it != Locs.end(); it++)
	{
		if (path.find(it->path) != std::string::npos)
			break;
	}
	if (it == Locs.end())
		return 0;
	return (&(*it));
};

std::string ws::HTTPResponse::GET(ws::HTTPreq &req, ws::Connection &connection,
								  ws::Location *loc)
{
	std::string response;
	if (loc && loc->binPath.empty() == false)
	{
		CGI cgi(req.path, loc, connection);
		response = cgi.getResponse();
		return addHeader(response, req, "200");
	} else
	{
		if (connection.redirect)
		{
			connection.config.root.clear();
			req.path = req.path.substr(1);
		}
		response = responseFromRoot(req, connection.config, loc);
		return response;
	}
}


std::string ws::HTTPResponse::POST(ws::HTTPreq &req,
								   ws::Connection &connection,
								   ws::Location *loc)
{
	std::string tmp = connection.request;
	if (connection.mode == APPLICATION || tmp.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		std::cout << "POST_CGI\n";
		connection.mode = APPLICATION;
		return POST_CGI(req, connection, loc);
	}
	else if (connection.mode == MULTIPART || tmp.find("multipart/form-data") != std::string::npos)
	{
		std::cout << "POST_DATA\n";
		connection.mode = MULTIPART;
		return POST_DATA(req, connection, loc);
	}
	else
		return errorPage("500", connection.config, loc, req);
}

std::string ws::HTTPResponse::POST_CGI(ws::HTTPreq &req,
									   ws::Connection &connection,
									   ws::Location *loc)
{
	std::string response;
	if (loc &&
		(req.path.find(".php") != std::string::npos || req.path.find(".py")
													   != std::string::npos))
	{
		CGI cgi(req.path, loc, connection);
		response = cgi.getResponse();
		return addHeader(response, req, "200");
	}
	return errorPage("500", connection.config, loc, req);
}

std::string ws::HTTPResponse::POST_DATA(ws::HTTPreq &req,
										ws::Connection &connection,
										ws::Location *loc)
{
	if (connection.uploadFile._path.empty())
	{
		connection.uploadFile._path = std::getenv("PWD");
		if (loc)
			connection.uploadFile._path = loc->root + loc->uploadPath;
		else
		{
			if (connection.config.uploadPath.empty())
			{
				std::cout << "UPLOAD PATH NOT DEFINED";
				return errorPage("500", connection.config, loc, req);
			}
			connection.uploadFile._path =
					connection.config.root + connection.config.uploadPath;
		}
	}

	std::string tmp = connection.request;
	if (!connection.isUploadStarted)
	{
		if (connection.uploadFileBoundary.empty())
		{
			size_t startPosition = tmp.find("boundary=");

			if (startPosition < tmp.length())
			{
				tmp = tmp.substr(startPosition + 9);
				std::string::iterator it;
				for (it = tmp.begin(); *it && *it != '\n'; it++);
				connection.uploadFileBoundary = std::string(tmp.begin(), it);
			}

//			std::cout << "\033[1;31m$$$$$" << std::endl;
//			std::cout << connection.uploadFileBoundary << std::endl;
//			std::cout << "$$$$$\033[0m" << std::endl;
		} else if (!connection.uploadFileBoundary.empty()
				   && connection.uploadFileName.empty())
		{
			size_t startPosition = tmp.find("filename=\"");
			if (startPosition < tmp.length())
			{
				tmp = tmp.substr(startPosition + 10);
				std::string::iterator it;
				for (it = tmp.begin(); *it && *it != '\"'; it++);
				connection.uploadFileName = std::string(tmp.begin(), it);
				connection.uploadFile._path.append(connection.uploadFileName);
			}
//			std::cout << "\033[1;32m$$$$$" << std::endl;
//			std::cout << connection.uploadFileName << std::endl;
//			std::cout << "$$$$$\033[0m" << std::endl;

			startPosition = tmp.find("\r\n\r\n");
			if (startPosition < tmp.length())
				tmp = tmp.substr(startPosition + 4);
			connection.isUploadStarted = true;

//			std::cout << "\033[1;33m$$$$$" << std::endl;
//			std::cout << tmp << std::endl;
//			std::cout << "$$$$$\033[0m" << std::endl;
		}
	}

	if (connection.isUploadStarted)
	{
		tmp = Split(tmp, "--" + connection.uploadFileBoundary);

//		std::cout << "POST#POST#POST#POST#POST#POST#POST#POST#POST"
//				  << std::endl;
//		std::cout << connection.uploadFile._path << std::endl;
//		std::cout << connection.request << std::endl;

		if (connection.uploadFile._fd == -1)
		{
			connection.uploadFile._fd = open(
					connection.uploadFile._path.c_str(),
					O_CREAT | O_RDWR | O_APPEND,
					0755
			);
			connection.uploadFile._fileOperation = WRITE_FILE;
		}
		connection.uploadFile.addToFile(tmp);

		if (tmp.length() < connection.request.length() &&
			connection.isUploadStarted)
		{
			connection.isUploadComplete = true;

			std::cout << "\033[1;31m$$$$$" << std::endl;
			std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
			std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
			std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
			std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
			std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
			std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
			std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
			std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
			std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
			std::cout << "$$$$$\033[0m" << std::endl;
		}
	}
//	if (connection.isUploadComplete)
//		return "HTTP/1.1 200 Ok/r/n";
//	else
	return std::string();
};

std::string
ws::HTTPResponse::DELETE(ws::HTTPreq &req, ws::Connection &connection,
						 ws::Location *loc)
{
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

std::string ws::HTTPResponse::redirect( Location *loc)
{
	std::string resp;

	resp.append("HTTP/1.1 ");
	resp.append(loc->redirect.begin()->first);
	resp.append("\r\n");
	resp.append("Location: ");
	resp.append("http://" + loc->redirect.begin()->second);
	resp.append("\r\n");
	resp.append("Content-Type: */*\n");
	resp.append("Content-Length: 0");
	resp.append("\r\n");
	resp.append("\r\n");
	return resp;
}
std::string ws::HTTPResponse::load(HTTPreq &req, Connection &connection)
{
	std::string response;

	ws::Location *loc = findLocation(req.path, connection.config.Locations);

	if (loc)
	{
		std::cout << *loc;
		req.host = loc->redirect.begin()->second;
		if (loc->redirect.empty() == false)
		{
			connection.redirect = true;
			connection.HTTPreq.connect = CLOSE;
			return redirect(loc);
		}
	}
	else
		std::cout << "NO LOCATION\n";
	std::cout << (connection.config.method.find(req.method) == std::string::npos) << " BOOL METHOD\n";
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

inline std::string &
ws::HTTPResponse::trim(std::string &line, const std::string &trimmer)
{
	line.erase(line.find_last_not_of(trimmer) + 1);         //suffixing spaces
	line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
	return line;
};

std::string ws::HTTPResponse::errorPage(const std::string &err, ws::Config &cnf,
										ws::Location *loc, ws::HTTPreq &req)
{
	std::vector<uint8_t> response;
	std::string path;
	ws::File myFd;
	if (loc && loc->errorPage[err].empty() == false)
		path = loc->errorPage.at(err);
	else if (cnf.errorPage[err].empty() == false)
		path = cnf.errorPage.at(err);
	else if (err == "400")
		return badRequest();
	else if (err == "404")
		return notFound();
	std::cout << path << " ERROR PATH\n";
	myFd.setPath(path, OPEN_FILE);
	if (myFd._fd < 0)
		return notFound();
	response = myFd.readFile();
	std::string msg(response.begin(), response.end());
	return addHeader(msg, req, err);
}

std::string ws::HTTPResponse::notFound()
{
	std::string response, msg;
	response.append("HTTP/1.1 404 Not Found\r\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: 0\n\n");
//	response.append("Content-Length: ");
//	response.append("Not Found!\n"); // Добавить чтение их html 

	return (response);
}

std::string ws::HTTPResponse::badRequest()
{
	std::string response, msg;
	response.append("HTTP/1.1 405\r\n");
	response.append("Content-Type: text/html\n");
	response.append("Content-Length: 0\n\n");
//	response.append("Content-Length: ");
//	response.append(""); // Добавить чтение их html 

	return (response);
}

std::string ws::HTTPResponse::addHeader(std::string &msg, ws::HTTPreq &req,
										const std::string &code)
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
	if (accept.find(extension) == std::string::npos || req.path.find(".php") != std::string::npos)
		accept = "*/*";

	head.append(accept + "\n");
	head.append("Content-Length: ");
	head.append(std::to_string(msg.size()));
	head.append("\n\n");
	head.append(msg);
	return head;
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

std::string	ws::HTTPResponse::responseFromRoot(HTTPreq &req, Config &cnf, Location *loc)
{
	if (cnf.method.find(req.method) == std::string::npos)
		return errorPage("400", cnf, loc, req);
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
			return addHeader(msg, req, "200");
		}
	}
}

std::string ws::HTTPResponse::Split(std::string &line, std::string delimiter)
{
	size_t pos = 0;
	std::string token;
	pos = line.find(delimiter);
	if (pos == std::string::npos)
	{
		token = line;
		line.erase();
		return (this->trim(token, " \t\r"));
	}

	token = line.substr(0, pos);
	line.erase(0, pos + delimiter.length());
	line.append("\0");
	return (trim(token, " \t\r"));
}
