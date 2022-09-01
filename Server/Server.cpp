//
// Created by Dmitry Novikov on 08.08.2022.
//

#include "Server.hpp"
#include "../Network/Listening.hpp"

ws::Server::Server(int domain, int service, int protocol,
				   int port, u_long ip, int backlog)
{ _listeningSocket = new Listening(domain, service, protocol, port, ip, backlog); }

ws::Listening *ws::Server::getListeningSocket()
{ return _listeningSocket; }
