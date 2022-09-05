//
// Created by Dmitry Novikov on 05.09.2022.
//

#include "Responder.hpp"

ws::Responder::Responder(ws::Client client)
{
	std::cout << client.getResponse() << std::endl;

	std::string responsePart;

	client.setBytesSent(
			send(
					client.getClientSocket(),
			client.getResponse().c_str() + client.getBytesSent(),
			client.getResponse().length() - client.getBytesSent(),
//					client.getResponse().c_str() + client.getBytesSent(),
//					client.getBufferSize(),
					0
			)
	);
//	if (client._bytesAlreadySent >= client._responseLength)
//	{
//		client._bytesAlreadySent = 0;
//		return true;
//	}
//	return false;
}
