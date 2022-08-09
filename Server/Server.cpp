//
// Created by Dmitry Novikov on 08.08.2022.
//

#include "Server.hpp"
#include "../Network/Listening.hpp"

ws::Server::Server(int domain, int service, int protocol,
				   int port, u_long ip, int backlog)
{
	_socket = new Listening(domain, service, protocol, port, ip, backlog);
}

ws::Listening *ws::Server::getSocket()
{ return _socket; }
//
//int main()
//{
//	int domain = AF_INET;
//	int service = SOCK_STREAM;
//	int protocol = 0;
//	int port = 8080;
//	u_long ip = INADDR_ANY;
//	int backlog = 100;
//
//	std::cout << ">>> START >>>" << std::endl;
////	std::cout << ">>> BINDING >>>" << std::endl;
//	ws::Binding(domain, service, protocol, port, ip);
//	std::cout << ">>> BINDING [SUCCESS] >>>" << std::endl;
//	std::cout << ">>> LISTENING >>>" << std::endl;
//	ws::Listening(domain, service, protocol, port, ip, backlog);
//	std::cout << ">>> LISTENING [SUCCESS] >>>" << std::endl;
//	std::cout << ">>> SERVER WAS SUCCESSFULLY STARTED >>>" << std::endl;
//
//
//}
