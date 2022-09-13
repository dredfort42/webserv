//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "CGI.hpp"

namespace ws
{

	CGI::CGI(std::string path)
	{
		File file = File(path, OPEN_FILE);
		_response.clear();

		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
		std::cout << path << std::endl;
//		std::cout << file._fileType << std::endl;
		if (file._fileType == ".php")
			_executableFile = "/usr/local/bin/php-cgi";
		else if (file._fileType == ".py")
			_executableFile = "/Library/Frameworks/Python.framework/Versions/3"
							 ".10/bin/python3";
//		std::cout << _executableFile << std::endl;

		_response = "TESTTESTTEST";

	}

	std::string CGI::getResponse()
	{
		return _response;
	}

} // ws
