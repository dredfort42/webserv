//
// Created by Dmitry Novikov on 07.09.2022.
//

#pragma once

#include <ctime>
#include "../Service/Service.hpp"
#include "../File/File.hpp"

namespace ws
{


	enum POST_MODE {
		APPLICATION,
		MULTIPART,
		TEXT,
		NONE,
	};

	struct Connection
	{
		Connection(int socket, Config &config);

		bool			isReadyToClose;
		int 			socket;
		std::string 	request;
		std::string 	response;
		size_t			bytesSent;
		std::time_t 	startTime;
		HTTPreq			HTTPreq;
		Config			config;
		File			uploadFile;
		std::string 	uploadFileName;
		std::string 	uploadFileBoundary;
		bool			isUploadStarted;
		bool 			isUploadComplete;
		bool 			redirect;
		bool 			isCGIStarted;
		POST_MODE		mode;
		resultConfig	setConfig;
	};

} // ws
