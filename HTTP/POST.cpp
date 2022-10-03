#include "HTTPResponse.hpp"
//#include "../Server/CGI/CGI.hpp"

namespace ws{

//	std::string HTTPResponse::POST(HTTPreq &req, Connection &connection, Location *loc)
//	{
////		if (connection.mode == MULTIPART || connection.request.find("multipart/form-data") != std::string::npos)
////		{
//	//		std::cout << "-----------------------\n" << connection.request << "\n----------------\nREQEST\n-------------\n";
//			connection.mode = MULTIPART;
//	//		std::cout << "CONNECTION POST_DATA\n";
//			std::string response = POST_DATA(req, connection, loc);
//	//		std::cout << "----------------------------\n"<< response << "\n----------------\nRESPONSE\n-------------\n";
//			return response;
////		}
////		else
////			return errorPage("500", connection.setConfig, loc, req);
//	}
//
//
//	std::string HTTPResponse::POST_DATA(HTTPreq &req,
//										Connection &connection,
//										Location *loc)

	std::string HTTPResponse::POST(HTTPreq &req,
			Connection &connection,
			Location *loc)
	{
		connection.mode = MULTIPART;
//		void(req.port);
//		void(loc->root);

		std::cout << "\n\n \033[0;33m \n";
		std::cout << "File Boundary: " << connection.uploadFileBoundary << "\n";
		std::cout << "File Name: " << connection.uploadFileName << "\n";
		std::cout << "Upload Started: " << (connection.isUploadStarted ? "YES" : "") << "\n";
		std::cout << "Upload Complete: " << (connection.isUploadComplete ? "YES" : "") << "\n";
		std::cout << "\033[0m\n\n";

//		std::cout << "\n\n \033[3;34m \n";
//		std::cout << connection.request << std::endl;
//		std::cout << "\n\n \033[0m \n";


		if (connection.uploadFile._path.empty())
		{
			connection.uploadFile._path = std::getenv("PWD");
			if (loc)
				connection.uploadFile._path = loc->root + loc->uploadPath;
			else
			{
				if (connection.setConfig.uploadPath.empty())
				{
					std::cout << "UPLOAD PATH NOT DEFINED";
					return errorPage("500", connection.setConfig, loc, req);
				}
				connection.uploadFile._path =
					connection.setConfig.root + connection.setConfig.uploadPath;
			}
		}

		std::string tmp = connection.request;

		if (connection.uploadFileBoundary.empty())
		{
			size_t startPosition = tmp.find("boundary=");

			if (startPosition < tmp.length())
			{
				tmp = tmp.substr(startPosition + 9);
				std::string::iterator it;
				for (it = tmp.begin(); *it && *it != '\n'; it++);
				connection.uploadFileBoundary = std::string(tmp.begin(), it);
			}

			std::cout << "\n"
						 "uploadFileBoundary_uploadFileBoundary_uploadFileBoundary"
						 "\033[1;31m"
						 "\n";
			std::cout << connection.uploadFileBoundary << std::endl;
			std::cout << "\033[0m\n\n";
		}

		if (connection.uploadFileName.empty() && !connection.uploadFileBoundary.empty())
		{
			size_t startPosition = tmp.find("filename=\"");
			if (startPosition < tmp.length())
			{
				tmp = tmp.substr(startPosition + 10);
				std::string::iterator it;
				for (it = tmp.begin(); *it && *it != '\"'; it++);
				connection.uploadFileName = std::string(tmp.begin(), it);
				connection.uploadFile._path.append(connection.uploadFileName);
			}

			std::cout << "\n"
						 "uploadFileName_uploadFileName_uploadFileName"
						 "\033[1;31m"
						 "\n";
			std::cout << connection.uploadFileName << std::endl;
			std::cout << "\033[0m\n\n";
		}

		if (!connection.isUploadStarted && !connection.uploadFileName.empty())
		{
			size_t startPosition = tmp.find("\r\n\r\n");
			if (startPosition < tmp.length())
				tmp = tmp.substr(startPosition + 4);
			connection.isUploadStarted = true;

			std::cout << "\n"
						 "isUploadStarted_isUploadStarted_isUploadStarted"
						 "\033[1;31m"
						 "\n";
			std::cout << (connection.isUploadStarted ? "YES" : "") << std::endl;
			std::cout << "\033[0m\n\n";
		}

		if (!connection.isUploadComplete && connection.isUploadStarted)
		{
			if (tmp.find(connection.uploadFileBoundary) != std::string::npos)
			{
				connection.isUploadComplete = true;
				tmp = Split(tmp, "--" + connection.uploadFileBoundary);
			}

			if (connection.uploadFile._fd == -1)
			{
				connection.uploadFile._fd = open(
						connection.uploadFile._path.c_str(),
						O_CREAT | O_RDWR | O_APPEND,
						0755
						);
				connection.uploadFile._fileOperation = WRITE_FILE;
			}
			connection.uploadFile.addToFile(tmp);

			std::cout << "\n"
						 "isUploadComplete_isUploadComplete_isUploadComplete"
						 "\033[1;31m"
						 "\n";
			std::cout << (connection.isUploadComplete ? "YES" : "") << std::endl;
			std::cout << "\033[0m\n\n";
		}

//
//		std::string tmp = connection.request;
//		if (!connection.isUploadStarted)
//		{
//			if (connection.uploadFileBoundary.empty())
//			{
//				size_t startPosition = tmp.find("boundary=");
//
//				if (startPosition < tmp.length())
//				{
//					tmp = tmp.substr(startPosition + 9);
//					std::string::iterator it;
//					for (it = tmp.begin(); *it && *it != '\n'; it++);
//					connection.uploadFileBoundary = std::string(tmp.begin(), it);
//				}
//
//			} else if (!connection.uploadFileBoundary.empty()
//					&& connection.uploadFileName.empty())
//			{
//				size_t startPosition = tmp.find("filename=\"");
//				if (startPosition < tmp.length())
//				{
//					tmp = tmp.substr(startPosition + 10);
//					std::string::iterator it;
//					for (it = tmp.begin(); *it && *it != '\"'; it++);
//					connection.uploadFileName = std::string(tmp.begin(), it);
//					connection.uploadFile._path.append(connection.uploadFileName);
//				}
//
//				startPosition = tmp.find("\r\n\r\n");
//				if (startPosition < tmp.length())
//					tmp = tmp.substr(startPosition + 4);
//				connection.isUploadStarted = true;
//
//			}
//		}
//
//		if (connection.isUploadStarted)
//		{
//			if (tmp.find("--" + connection.uploadFileBoundary) !=
//			std::string::npos)
//			{
//				connection.isUploadComplete = true;
//				tmp = Split(tmp, "--" + connection.uploadFileBoundary);
//			}
//
//			if (connection.uploadFile._fd == -1)
//			{
//				connection.uploadFile._fd = open(
//						connection.uploadFile._path.c_str(),
//						O_CREAT | O_RDWR | O_APPEND,
//						0755
//						);
//				connection.uploadFile._fileOperation = WRITE_FILE;
//			}
//			connection.uploadFile.addToFile(tmp);
//			usleep(500);
//			if (connection.isUploadComplete &&
//					connection.isUploadStarted)
//			{
//				//	std::cout <<"------------------------------------\n";
//				//	std::cout << tmp << "\n";
//				//	std::cout <<"------------------------------------\n";
//				////	std::cout << connection.request << "\n";
//				//	std::cout << connection.request.length() << " REQUEST SIZE\n";
//				//	std::cout << tmp.length() << " TMP SIZE\n";
//				//	std::cout <<"------------------------------------\n";
//
//
//
//				std::cout << "\033[1;31m$$$$$" << std::endl;
//				std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
//				std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
//				std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
//				std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
//				std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
//				std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
//				std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
//				std::cout << "_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF" << std::endl;
//				std::cout << "EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_EOF_" << std::endl;
//				std::cout << "$$$$$\033[0m" << std::endl;
//
////				connection.isUploadStarted = false;
//			}
//		}
//		std::cout << "==============================================\n";
//		std::cout << tmp << std::endl;


		return std::string();
	};

};
