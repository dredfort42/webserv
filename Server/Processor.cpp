//
// Created by Dmitry Novikov on 08.09.2022.
//

#include "Server.hpp"

namespace ws
{

	resultConfig Server::makeConfig(Connection &c)
	{
		resultConfig res;
		std::string id = c.HTTPreq.host;

		res.ip = c.config.ip;
		res.port = c.config.port;
		res.serverName = id;
		res.autoindex = c.config.setup[id].autoindex;
		res.root = c.config.setup[id].root;
		res.method = c.config.setup[id].method;
		res.index = c.config.setup[id].index;
		res.uploadPath = c.config.setup[id].uploadPath;
		res.bodySize = c.config.setup[id].bodySize;
		res.errorPage = c.config.setup[id].errorPage;
		res.Locations = c.config.setup[id].Locations;
		return res;
	}

	void Server::processor(Connection &connection)
	{
		ws::HTTPResponse response;
		std::cout << connection.HTTPreq.path << " PATH\n";
		try {
			ws::HTTPparser req(connection.request);

			std::cout << req.getRequest();
			connection.HTTPreq = req.getRequest();

			std::string resp = response.load(connection.HTTPreq, connection);
			connection.response = resp;

		}
		catch (const std::exception& ex)
		{
		//	std::cout << ex.what() << "\n";
			std::string resp = response.load(connection.HTTPreq, connection);
			if (connection.isUploadComplete)
			{
			//	connection.HTTPreq.connect = CLOSE;
				connection.mode = NONE;
				connection.response = "HTTP/1.1 303 See Other\r\nLocation: " + connection.HTTPreq.path + "\r\n\r\n";
			}
		}


		ws::HTTPResponse response;
		connection.setConfig =  makeConfig(connection);
		std::string resp = response.load(connection.HTTPreq, connection);
		connection.response = resp;
		
		//	std::cout << connection.response;
		//// TMP /////////////////////////////////////////////////////////
	//	std::string message;
	//	message.append("Response to socket: ");
	//	message.append(std::to_string(connection.socket));
	//	message.append("\nHello client\0");

	//	connection.response.append("HTTP/1.1 200 OK\n");
	//	connection.response.append("Content-Type: text/plain\n");
	//	connection.response.append("Content-Length: ");
	//	connection.response.append(std::to_string(message.length()));
	//	connection.response.append("\n\n");
	//	connection.response.append(message);

		//// END /////////////////////////////////////////////////////////

		// Process complete
		FD_SET(connection.socket, &_masterWriteSet);

		std::cout << "\033[33m[PROCESSOR]\033[0m Socket: ";
		std::cout << connection.socket << std::endl;
	}

} // ws
