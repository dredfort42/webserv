//
// Created by Dmitry Novikov on 07.09.2022.
//

#include "Connection.hpp"

namespace ws
{

	Connection::Connection(int socket, ws::Config &config)
	{
		isReadyToClose = false;
		this->socket = socket;
		this->config = config;
		request = "";
		response = "";
		bytesSent = 0;
		uploadFile = File();
		uploadFileName.clear();
		uploadFileBoundary.clear();
		isUploadStarted = false;
		isUploadComplete = false;
		redirect = false;
		isCGIStarted = false;
		
		time(&startTime);
		
	}

} // ws
