//
// Created by Dmitry Novikov on 05.09.2022.
//

#pragma once

#include <sys/socket.h>
#include "Client.hpp"

namespace ws
{

	class Handler
	{
	private:
		Handler();

	public:
		Handler(Client &client);

	};

} // ws
