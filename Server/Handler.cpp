//
// Created by Dmitry Novikov on 05.09.2022.
//

#include "Handler.hpp"

ws::Handler::Handler(Client &client)
{
	char buffer[client.getBufferSize() + 1];

	memset(&buffer, 0, sizeof(buffer));
	client.setBytesReceived(
			recv(client.getClientSocket(), buffer, client.getBufferSize(), 0)
	);
	if (client.getBytesReceived() > 0)
		client.setRequest(buffer);
	//// TMP /////////////////////////////////////////////////////////
	else
		std::cout << client.getRequest() << std::endl;
	//// END /////////////////////////////////////////////////////////
}
