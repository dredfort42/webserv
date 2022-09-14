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
			std::string	notFound();
			std::string badRequest();
			std::string addHeader(std::string& msg, ws::HTTPreq& req);
			std::string	Split(std::string &line, std::string delimiter);
			
			std::string	responseFromRoot(HTTPreq &req, Config &cnf);

			inline std::string& trim( std::string &line, const std::string &trimmer);
		private:
		//	void	*res;
		//	size_t	size;
	};
}
