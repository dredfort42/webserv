//
// Created by Dmitry Novikov on 04.09.2022.
//

#include <iostream>

#include "Config/Parser.hpp"
#include "Server/WebServer.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << argv[0] <<  ": Wrong number of args!" << std::endl;
		return (EXIT_FAILURE);
	}
	ws::Parser parseConf(argv[1]);
	std::vector<ws::Config> tokens;
	try {
		tokens = parseConf.getStruct();
	}
	catch (const std::exception& e){
		std::cerr << e.what();
		return (EXIT_FAILURE);
	}
	ws::WebServer webServer(tokens);
	webServer.startWebServer();
}
