//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "CGI.hpp"

#define PIPE_IN 1
#define PIPE_OUT 0

namespace ws
{

	CGI::CGI(std::string &path, int &clientSocket)
	{
		std::string tmpFilePath = "/Users/dnovikov/Desktop/webserv/Server/CGI"
								  "/tmp/";
		tmpFilePath.append(std::to_string(clientSocket));

		_tmpFile = File(tmpFilePath, CREATE_FILE);
		std::cout << "\033[1;32m >>> TMP FILE CREATED >>> \033[0m" << std::endl;
		_path = path;
		_absolutePath = "/Users/dnovikov/Desktop/webserv/www/server3";
		_absolutePath.append(path);
		_requestArguments.clear();
		_response.clear();

		//// TMP ////////////////////////////////
//		File file = File(path, OPEN_FILE);
//		if (file._fileType == ".php")
		_executableFile = "/usr/local/bin/php-cgi";
//		else if (file._fileType == ".py")
//			_executableFile = "/Library/Frameworks/Python.framework/Versions/3"
//							 ".10/bin/python3";
		//// END ////////////////////////////////


		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
//		std::cout << _absolutePath << std::endl;
//		std::cout << _executableFile << std::endl;

//		_response = "TESTTESTTEST";
		execute();
	}

	int CGI::execute()
	{
		char *argv[] = {const_cast<char *>(_executableFile.c_str()),
						const_cast<char *>(_absolutePath.c_str()),
						const_cast<char *>(_requestArguments.c_str()),
						NULL};

		std::cout << "\033[1;32m >>> " << argv[0] << " >>> \033[0m"
				  << std::endl;
		std::cout << "\033[1;32m >>> " << argv[1] << " >>> \033[0m"
				  << std::endl;
		std::cout << "\033[1;32m >>> " << argv[2] << " >>> \033[0m"
				  << std::endl;

		int pipeFd[2];

		if (pipe(pipeFd) != 0)
			return 500;
		pid_t pid = fork();
		if (!pid)
		{
			close(pipeFd[PIPE_IN]);
			if (dup2(pipeFd[PIPE_OUT], STDIN_FILENO) == -1)
				return 500;
			if (dup2(_tmpFile._fd, STDOUT_FILENO) == -1)
				return 500;
			close(pipeFd[PIPE_OUT]);
			execve(argv[0], argv, NULL);
			exit(1);
		} else if (pid > 0)
		{
			close(pipeFd[PIPE_OUT]);
			close(pipeFd[PIPE_IN]);

			int status;

			if (waitpid(pid, &status, 0) == -1)
				return 500;
			if (WIFEXITED(status) && WEXITSTATUS(status))
				return 502;
		} else
			return 502;

		std::vector<uint8_t> tmpData = _tmpFile.readFile();
		if (remove(_tmpFile._path.c_str()))
			return 500;
		std::cout << "\033[1;32m >>> TMP FILE DELETED >>> \033[0m" << std::endl;
		_response = std::string(tmpData.begin(), tmpData.end());
		return 200;
	}

	std::string CGI::getResponse()
	{
		return _response.substr(_response.find("<html>"));;
	}

} // ws
