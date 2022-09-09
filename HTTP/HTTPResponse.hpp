#pragma once
#include <iostream>
#include "HTTPparser.hpp" 
#include "../Config/Parser.hpp"

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
		
			std::string	load(HTTPreq &req, Config &cnf);
			std::string	comparePaths(HTTPreq &req, Config &cnf);
			
	};
}
