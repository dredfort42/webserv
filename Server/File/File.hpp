//
// Created by Dmitry Novikov on 11.09.2022.
//

#pragma once

#include <string>
#include <fcntl.h>

namespace ws
{
	enum MimeType
	{
		PHP,
	};

	enum FileOperation
	{
		OPEN_FILE,
		CREATE_FILE,
		ADD_DATA
	};

	struct File
	{
		int 		_fd;
		std::string _path;
//		std::string _fileName;
		MimeType	_fileType;

//		File();
		File(std::string path, FileOperation operation);

		static MimeType getFileType(std::string path);
	};

}
