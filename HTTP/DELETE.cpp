#include "HTTPResponse.hpp"
#include "../Server/CGI/CGI.hpp"

namespace ws{


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
				if (connection.setConfig.uploadPath.empty())
				{
					std::cout << "UPLOAD PATH NOT DEFINED";
					return errorPage("500", connection.setConfig, loc, req);
				}
				path = connection.setConfig.root + connection.setConfig.uploadPath;
			}
			if (req.path.find(path) == std::string::npos)
				return addHeader(response, req, "204");
			path += std::string(req.path.substr(req.path.rfind("/") + 1));
			std::cout << path;
			ws::File myFd(path, OPEN_FILE);
			if (myFd._fd < 0)
				return addHeader(response, req, "204");
			myFd.removeFile();
			return addHeader(response, req, "202");

		}
};
