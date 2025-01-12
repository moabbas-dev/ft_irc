#ifndef BOT_HPP
# define BOT_HPP

#include "Cmd.hpp"
#include "StringUtils.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <cstdlib>
#include <ctime>

class Bot : public Client
{
	public:
		Bot();
		~Bot();

		void connectToServer(Server &server);
		void sendAgeMsg(Client& client, const std::string &command);
		void sendJokeMsg(Client& client);
		void sendHelpMsg(Client& client);
		void sendUnknowCmdMsg(Client& client);
};

#endif