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
		std::string _response;
		std::string _executableFile;

		CGI();

	public:
		CGI(std::string path);

		std::string getResponse();

	};

} // ws
