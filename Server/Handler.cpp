//
// Created by Dmitry Novikov on 05.09.2022.
//

#include "Handler.hpp"

ws::Handler::Handler(Client client)
{
	//// TMP /////////////////////////////////////////////////////////
	std::string message;
	message.append("Hello client");

	std::string response;
	response.append("HTTP/1.1 200 OK\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: ");
	response.append(std::to_string(message.length()));
	response.append("\n\n");
	response.append(message);
	//// END /////////////////////////////////////////////////////////

	char buffer[client.getBufferSize()];

	memset(buffer, 0, sizeof(buffer));
	client.setBytesReceived(
		recv(client.getClientSocket(), buffer, client.getBufferSize(), 0)
	);
	if (client.getBytesReceived() > 0)
	{
		client.setRequest(buffer);
		std::cout << client.getRequest() << std::endl;
	}

	//// TMP /////////////////////////////////////////////////////////
	else
		client.setResponse(response);
	//// END /////////////////////////////////////////////////////////
}
