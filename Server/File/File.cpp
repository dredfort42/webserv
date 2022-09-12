//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "File.hpp"
#include <iostream>
namespace ws
{
	File::File(std::string path, FileOperation operation)
	{
//		path = "/" + path;
//		path = std::getenv("PWD") + path;
		std::cout << path << " | PATH\n";
		switch (operation)
		{
			case OPEN_FILE:
				this->_fd = open(path.c_str(), O_RDONLY | O_NONBLOCK); break;
			case CREATE_FILE:
				this->_fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC); break;
			default:
				this->_fd = 0; break;
		}
		std::cout << this->_fd << " FD\n";
		if (this->_fd > 0)
		{
			this->_path = path;
//			_fileName = fileName;
			this->_fileType = getFileType(path);
		}

	}
	
	std::string	File::readAll() 
	{
		std::string msg;
		char buf[1024];
		size_t p = 1;
		if (this->_fd <= 0)
		{
			close(this->_fd);
			return msg;
		}
		while ((p = read(this->_fd, buf, 1024)) > 0)
			msg += buf;
		std::cout << msg << "\n READED FILE\n";
		close(this->_fd);
		return msg;
	}
	MimeType File::getFileType(std::string fileName)
	{
		(void)fileName;
		return PHP;
	}
} // ws
