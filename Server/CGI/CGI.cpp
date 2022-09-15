//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "CGI.hpp"

#define PIPE_IN 1
#define PIPE_OUT 0

namespace ws
{

	CGI::CGI(std::string &path, int &clientSocket, Location *loc)
	{
		std::string tmpFilePath = std::getenv("PWD");
		tmpFilePath.append("/Server/CGI/tmp/");
		tmpFilePath.append(std::to_string(clientSocket));
		tmpFilePath.append("_CGI_");
		tmpFilePath.append(std::to_string(clock()));

		_tmpFile = File(tmpFilePath, CREATE_FILE);
		std::cout << "\033[1;32m >>> TMP FILE CREATED >>> \033[0m" << std::endl;

		_commandLine = std::string(std::getenv("PWD"));
		_commandLine.append("/");
		_commandLine.append(loc->root);
		_commandLine.append(path);
//		std::cout << _commandLine << std::endl;
		_executableFile = loc->binPath;
		_response.clear();

		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
		_code = executor();
	}

	int CGI::executor()
	{
		std::string pathToFileToExecute = split(_commandLine, "?");
		std::string arguments = _commandLine;
		char *argv[] = {const_cast<char *>(_executableFile.c_str()),
						const_cast<char *>(pathToFileToExecute.c_str()),
						const_cast<char *>(arguments.c_str()),
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
			if (execve(argv[0], argv, NULL) == -1)
				return 404;
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
		if (_tmpFile.removeFile())
			return 500;
		std::cout << "\033[1;32m >>> TMP FILE DELETED >>> \033[0m" << std::endl;
		_response = std::string(tmpData.begin(), tmpData.end());
		return 200;
	}

	std::string CGI::getResponse()
	{
		const size_t pos = _response.find("<html>");
		if (pos != std::string::npos)
			return _response.substr(pos);
		return _response;
	}

	std::string	CGI::split(std::string &line, std::string delimiter)
	{
		size_t pos = 0;
		std::string token;
		pos = line.find(delimiter);
		if (pos == std::string::npos)
		{
			token = line;
			line.erase();
			return (this->trim(token, " \t"));
		}

		token = line.substr(0, pos);
		line.erase(0, pos + delimiter.length());
		line.append("\0");
		return (trim(token, " \t"));
	};

	inline std::string &CGI::trim( std::string &line, const std::string
	&trimmer)
	{
		line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
		line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
		return line;
	}

	std::string CGI::getCode()
	{
		return std::to_string(_code);
	};
} // ws
