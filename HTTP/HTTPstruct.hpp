#pragma once
#include <iostream>
#include <map>
namespace ws {
 	enum STATUS{
			KEEP_ALIVE,
			CLOSE,
			GET,
			POST,
			DELETE,
			EMPTY
	};
	struct HTTPreq {
		std::string	method;
		std::string path;
		std::string host;	
		std::string port;
		STATUS		connect;
		std::string accept; //Создать генерацию mime типов для респонда?
		std::string acceptEnc;
		std::string acceptLang;
		std::string User_Agent;
	//	std::map<std::string, std::string> variables;

		HTTPreq() {
			method = EMPTY;
			connect = EMPTY;
		};
	};
}
std::ostream&	operator<<(std::ostream &o, ws::HTTPreq const &req);
