#ifndef BOT_HPP
# define BOT_HPP

#include "Server.hpp"
#include "Cmd.hpp"
#include "StringUtils.hpp"
#include "Client.hpp"

class Bot : public Client
{
	public:
		Bot();
		~Bot();

		void connectToServer(Server &server);
};

#endif