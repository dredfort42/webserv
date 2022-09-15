//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "File.hpp"

namespace ws
{

	File::File()
	{
		_fd = -1;
		_fileOperation = CLOSE_FILE;
		_path.clear();
		_fileType.clear();
	}

	File::File(const std::string &path, const FileOperation &operation)
	{
		switch (operation)
		{
			case OPEN_FILE:
				_fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
				_fileOperation = OPEN_FILE;
				break;
			case CREATE_FILE:
				_fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC);
				_fileOperation = CREATE_FILE;
				break;
			default:
				_fd = 0;
		}
		if (_fd > 0)
		{
			_path = path;
			_fileType = getFileType(const_cast<std::string &>(path));
		}

	}

	void		File::setPath(std::string &path, const FileOperation &operation)
	{
		if (_fd > 0)
			closeFile();

		switch (operation)
		{
			case OPEN_FILE:
				_fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
				_fileOperation = OPEN_FILE;
				break;
			case CREATE_FILE:
				_fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC);
				_fileOperation = CREATE_FILE;
				break;
			default:
				_fd = 0;
		}
		if (_fd > 0)
		{
			_path = path;
			_fileType = getFileType(const_cast<std::string &>(path));
		}
	}

	std::string File::getFileType(std::string &path)
	{
		std::string fileType;
		fileType.clear();
		std::string::iterator lastDot;

		for (std::string::iterator it = path.end();
		it != path.begin() && *it != '.' && *it != '/';
		it--)
			lastDot = it;
		if ( *lastDot-- && *lastDot != '/' && lastDot != path.begin())
			fileType = std::string(lastDot + 1, path.end());
		return fileType;

//		return fileName.substr(fileName.find_last_of('.'));
	}

	void File::closeFile()
	{
		if (_fd > -1)
			close(_fd);
		_fileOperation = CLOSE_FILE;
	}

	void File::addToFile(const std::string &data)
	{
		if (_fileOperation != WRITE_FILE)
		{
			closeFile();
			_fd = open(_path.c_str(), O_RDWR | O_APPEND);
			if (_fd > -1)
				_fileOperation = WRITE_FILE;
		}
		if (_fileOperation == WRITE_FILE && !data.empty())
			write(_fd, data.c_str(), data.length());
	}

	std::vector<uint8_t> File::readFile()
	{
		std::vector<uint8_t> fileData;
		uint8_t buffer[WS_BUFFER_SIZE];
		int bytesWasRead;

		_fileOperation = READ_FILE;
		memset(&buffer, 0, sizeof(buffer));
		bytesWasRead = 0;
		lseek(_fd, 0, SEEK_SET);
		while ((bytesWasRead = read(_fd, buffer, WS_BUFFER_SIZE)) > 0)
			for (int i = 0; i < bytesWasRead; i++)
				fileData.push_back(buffer[i]);
		closeFile();
		return fileData;
	}

	int File::removeFile()
	{
		std::cout << "\033[1;33m >>> FILE DELETED >>> \033[0m" << std::endl;
		return remove(_path.c_str());
	}
} // ws
