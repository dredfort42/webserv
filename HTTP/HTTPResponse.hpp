#pragma once
#include <iostream>
#include "HTTPparser.hpp" 
#include "../Config/Parser.hpp"
#include "../Server/File/File.hpp"
#include "../Server/Connection/Connection.hpp"
namespace ws {
	enum status_code {
		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		REDIRECT = 307,
		NOT_FOUND = 404,
		BAD_REQUEST = 400,
		METHOD_NOT_ALLOWED = 405,
		INTERNAL_SERVER_ERROR = 500,
		HTTP_VERSION_NOT_SUPPORTED = 505
	};
	
	class HTTPResponse {
		public:
			HTTPResponse();
			~HTTPResponse();
		
			std::string	load(HTTPreq &req, Connection &connection);
			//std::string	comparePaths(HTTPreq &req, Config &cnf);
			Location*	findLocation(std::string &path, std::vector<Location> &Locations);

			//std::string	makeMessage(Location *loc, Config& cnf, HTTPreq& req);
			std::string errorPage(const std::string &err, resultConfig &cnf, Location *loc, HTTPreq &req);
			std::string errorTemplate(int err);
			std::string	notFound();
			std::string badRequest();
			std::string notAllowed();
			std::string internalError();
			std::string badGateway();
			std::string addHeader(std::string& msg, ws::HTTPreq& req, const std::string& code);
			std::string	Split(std::string &line, std::string delimiter);
			bool		endWith(std::string &str, std::string &end);
			
			std::string GET(HTTPreq &req, Connection &connection, Location *loc);
			std::string POST(HTTPreq &req, Connection &connection, Location *loc);
			std::string POST_CGI(HTTPreq &req, Connection &connection,
								 Location *loc);
			std::string POST_DATA(HTTPreq &req, Connection &connection,
							 Location *loc);
			std::string DELETE(HTTPreq &req, Connection &connection, Location *loc);
			std::string	responseFromRoot(HTTPreq &req, resultConfig &cnf, Location *loc);


			std::string redirect( Location *loc);
			inline std::string& trim( std::string &line, const std::string &trimmer);
		private:
		//	void	*res;
		//	size_t	size;
	};
}
