#ifndef BOT_HPP
# define BOT_HPP

#include "Cmd.hpp"
#include "StringUtils.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Bot : public Client
{
	public:
		Bot();
		~Bot();

		void connectToServer(Server &server);
		void sendAgeMsg(Client& client);
		void sendJokeMsg(Client& client);
		void sendHelpMsg(Client& client);
		void sendUnknowCmdMsg(Client& client);
};

#endif