//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "File.hpp"

namespace ws
{
	File::File(std::string path, FileOperation operation)
	{
		switch (operation)
		{
			case OPEN_FILE:
				_fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
			case CREATE_FILE:
				_fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC);
			default:
				_fd = 0;
		}

		if (_fd > 0)
		{
			_path = path;
//			_fileName = fileName;
			_fileType = getFileType(path);
		}

	}

	MimeType File::getFileType(std::string fileName)
	{
		(void)fileName;
		return PHP;
	}
} // ws
