//
// Created by Dmitry Novikov on 09.08.2022.
//

#ifndef WEBSERV_LISTENING_HPP
#define WEBSERV_LISTENING_HPP

# include "Binding.hpp"

namespace ws
{

	class Listening: public Binding
	{
	private:
		int _backlog;
		int _listening;

	public:
		Listening(int domain,
				int service,
				int protocol,
				int port,
				u_long ip,
				int backlog);

		void startListening();

	};

} // ws

#endif //WEBSERV_LISTENING_HPP
