#include "HTTPResponse.hpp"
#include "../Server/CGI/CGI.hpp"

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


/*

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
	return errorPage("500", connection.setConfig, loc, req);
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
			if (connection.setConfig.uploadPath.empty())
			{
				std::cout << "UPLOAD PATH NOT DEFINED";
				return errorPage("500", connection.setConfig, loc, req);
			}
			connection.uploadFile._path =
					connection.setConfig.root + connection.setConfig.uploadPath;
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
		//	std::cout <<"------------------------------------\n";
		//	std::cout << tmp << "\n";
		//	std::cout <<"------------------------------------\n";
		////	std::cout << connection.request << "\n";
		//	std::cout << connection.request.length() << " REQUEST SIZE\n";
		//	std::cout << tmp.length() << " TMP SIZE\n";
		//	std::cout <<"------------------------------------\n";

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
*/

std::string ws::HTTPResponse::load(HTTPreq &req, Connection &connection)
{
	std::string response;

	ws::Location *loc = findLocation(req.path, connection.setConfig.Locations);

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

	if (connection.uploadFileBoundary.empty() == false)
		return POST(req, connection, loc);

	std::cout << (connection.setConfig.method.find(req.method) == std::string::npos) << " BOOL METHOD\n";
	if (loc && loc->method.find(req.method) == std::string::npos)
	{
		std::cout << loc->method << " = " << req.method <<" | METHOD NOT FOUND IN LOCATION\n";
		return errorPage("400", connection.setConfig, loc, req);
	}
	else if (connection.setConfig.method.find(req.method) == std::string::npos)
	{
		std::cout << connection.setConfig.method << " = " << req.method <<" | METHOD NOT FOUND IN CONFIG\n";
		return errorPage("400", connection.setConfig, loc, req);
	}

	std::cout << "METHOD = " << req.method << "\n";	
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

std::string ws::HTTPResponse::errorPage(const std::string &err, ws::resultConfig &cnf,
										ws::Location *loc, ws::HTTPreq &req)
{
	std::vector<uint8_t> response;
	std::string path;
	ws::File myFd;
	std::cout<< "ERR " << err << "\n";
	if (loc && loc->errorPage[err].empty() == false)
	{
		std::cout << "Location ERROR PATH\n";
		path = loc->errorPage.at(err);
	}
	else if (cnf.errorPage[err].empty() == false)
	{
		std::cout << "Config  ERROR PATH\n";
		path = cnf.errorPage.at(err);
	}
	else if (err == "400")
		return badRequest();
	else if (err == "404")
		return notFound();
	else if (err == "502")
		return badGateway();
	else 
		return notFound();
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

	return (response);
}

std::string ws::HTTPResponse::badRequest()
{
	std::string response, msg;
	response.append("HTTP/1.1 405\r\n");
	response.append("Content-Type: text/html\n");
	response.append("Content-Length: 0\n\n");

	return (response);
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

std::string ws::HTTPResponse::badGateway()
{
	std::string response, msg;
	response.append("HTTP/1.1 502\r\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: 12\n\n");
	response.append("BAD GATEWAY\n"); 

	return (response);
}

std::string ws::HTTPResponse::addHeader(std::string &msg, ws::HTTPreq &req,
										const std::string &code)
{
	std::string head, extension, accept;

	extension = ws::File::getFileType(req.path);

	head.append("HTTP/1.1 ");
	head.append(code);
	head.append("\r\n");
	head.append("Content-Type: ");
	while (req.accept.empty() == false)
	{
		accept = Split(req.accept, ",");
		if (accept.find(extension) != std::string::npos)
			break;
	}
	if (accept.find(extension) == std::string::npos || req.path.find(".php") != std::string::npos)
		accept = "*/*";

	head.append(accept + "\r\n");
	head.append("Content-Length: ");
	head.append(std::to_string(msg.size()));
	head.append("\r\n\r\n");
	head.append(msg);
	return head;
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
		return (token);
	}

	token = line.substr(0, pos);
	line.erase(0, pos + delimiter.length());
	line.append("\0");
	return (token);
}
