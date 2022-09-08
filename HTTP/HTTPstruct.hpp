#pragma once
#include <iostream>

namespace ws {
	struct HTTPreq {
		std::string method;
		std::string path;
		std::string host;	
		std::string port;
		std::string connect;
		std::string accept; //Создать генерацию mime типов для респонда?
		std::string acceptEnc;
		std::string acceptLang;
		std::string User_Agent;
	};

}
std::ostream&	operator<<(std::ostream &o, ws::HTTPreq const &req);
