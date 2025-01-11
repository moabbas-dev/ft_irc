#include "../includes/Bot.hpp"
#include "../includes/Client.hpp"

Bot::Bot() : Client()
{
	this->nickname = "$marvin";
	this->username = "bot";
	this->realname = "Marvin the bot";
}

Bot::~Bot() { }

