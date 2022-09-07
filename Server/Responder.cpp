//
// Created by Dmitry Novikov on 05.09.2022.
//

#include "Responder.hpp"

ws::Responder::Responder(ws::Client client)
{
	//// TMP /////////////////////////////////////////////////////////
	std::string message;
	message.append("Hello client\0");

	std::string response;
	response.append("HTTP/1.1 200 OK\n");
	response.append("Content-Type: text/plain\n");
	response.append("Content-Length: ");
	response.append(std::to_string(message.length()));
	response.append("\n\n");
	response.append(message);

	client.setResponse(response);

//	std::cout << "___________________" << std::endl;
//	std::cout << client.getResponse() << std::endl;
//	std::cout << "___________________" << std::endl;
	//// END /////////////////////////////////////////////////////////

	int bytesToSend = client.getBufferSize();
	if (client.getResponse().length() + 1 < client.getBytesSent() + bytesToSend)
		bytesToSend = client.getResponse().length() - client.getBytesSent();

	client.setBytesSent(
			send(client.getClientSocket(),
				 client.getResponse().c_str() + client.getBytesSent(),
				 bytesToSend,
				 0
			)
	);
}
