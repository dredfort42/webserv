//
// Created by Dmitry Novikov on 11.09.2022.
//

#pragma once

#include <iostream>
#include "../File/File.hpp"
#include "../Connection/Connection.hpp"
#include "../../Config/ConfigStruct.hpp"


namespace ws
{

	class CGI
	{
	private:
		File 		_tmpFile;
		std::string _commandLine;
		std::string _executableFile;
		std::string _pathToFileToExecute;
		std::string _commandLineArguments;
		std::string _requestArguments;
		std::string _response;
		int 		_code;

		CGI();

	public:
		CGI(std::string &path, Location *loc, ws::Connection &connection);

		int 		executor();
		std::string getResponse();
		std::string	getCode();
		std::string	split(std::string &line, std::string delimiter);
		inline std::string &trim( std::string &line, const std::string
		&trimmer);
	};

} // ws
