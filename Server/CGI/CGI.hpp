//
// Created by Dmitry Novikov on 11.09.2022.
//

#pragma once

#include <iostream>
#include "../File/File.hpp"

namespace ws
{

	class CGI
	{
	private:
		File 		_tmpFile;
		std::string _path;
		std::string _absolutePath;
		std::string _requestArguments;
		std::string _executableFile;
		std::string _response;

		CGI();

	public:
		CGI(std::string &path, int &clientSocket);

		int 		execute();
		std::string getResponse();

	};

} // ws
