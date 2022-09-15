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
		std::string _commandLine;
		std::string _executableFile;
		std::string _response;
		int 		_code;

		CGI();

	public:
		CGI(std::string &path, int &clientSocket);

		int 		executor();
		std::string getResponse();
		std::string	getCode();
		std::string	split(std::string &line, std::string delimiter);
		inline std::string &trim( std::string &line, const std::string
		&trimmer);
	};

} // ws
