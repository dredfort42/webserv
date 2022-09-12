//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "CGI.hpp"

namespace ws
{

	CGI::CGI(std::string path)
	{
		File file = File(path, OPEN_FILE);
		_response = "";
	}

	std::string CGI::getResponse()
	{
		return std::string();
	}

} // ws
