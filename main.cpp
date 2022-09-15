//
// Created by Dmitry Novikov on 04.09.2022.
//
#include <iostream>

#include "Config/Parser.hpp"
#include "Server/Server.hpp"
#include <signal.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << argv[0] << ": Wrong number of args!" << std::endl;
		return (EXIT_FAILURE);
	}
//	signal(SIGPIPE, signal_callback_handler);
	ws::Parser parseConf(argv[1]);
	try
	{
		ws::Server(parseConf.getStruct()).run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what();
		return (EXIT_FAILURE);
	}
}
