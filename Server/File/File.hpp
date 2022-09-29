//
// Created by Dmitry Novikov on 11.09.2022.
//

#pragma once

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

#include "../Service/Service.hpp"

namespace ws
{

	enum FileOperation
	{
		OPEN_FILE,
		CREATE_FILE,
		READ_FILE,
		WRITE_FILE,
		CLOSE_FILE
	};

	struct File
	{
		int 			_fd;
		FileOperation	_fileOperation;
		std::string		_path;
		std::string		_fileType;

		File();
		File(const std::string &path, const FileOperation &operation);

		void					setPath(std::string &path,const FileOperation &op);
		static std::string 		getFileType(std::string &path);
		std::vector<uint8_t>	readFile();
		void 					addToFile(const std::string &data);
		int 					removeFile();
		void					closeFile();
	};

}
