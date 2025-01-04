/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:45:21 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/04 12:20:50 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/Errors.hpp"
#include "../includes/Cmd.hpp"
#include "../includes/Client.hpp"

std::map<int, std::string> Errors::errors;

void Errors::raise(Client &client, const std::string &msgName, int errorCode)
{
	std::string clientName = client.getHasSetNickName() ? client.getNickname() : client.getHostName();
	std::ostringstream result;
	result << ": " << errorCode << " ";
	result << msgName.empty()? clientName + " " : clientName + " " + msgName + " ";

	switch (errorCode)
	{
	case ERR_UNKNOWCOMMAND:
		result << ":Unknown Command";
		break;
	case ERR_NEEDMOREPARAMS:
		result << ":Not enough parameters";
		break;
	case ERR_TOOMANYPARAMS:
		result << ":Too many parameters";
		break;
	case ERR_PASSWDMISMATCH:
		result << ":Password incorrect";
		break;
	case ERR_ALREADYREGISTERED:
		result << ":You may not reregister";
		break;
	case ERR_NOTREGISTERED:
		result << ":You have not registered";
		break;
	case ERR_NONICKNAMEGIVEN:
		result << ":No nickname given";
		break;
	case ERR_ERRONEUSNICKNAME:
		result << ":Erroneus nickname";
		break;
	case ERR_NICKNAMEINUSE:
		result << ":Nickname is already in use";
		break;
	case ERR_TOOMANYCHANNELS:
		result << ":You have joined too many channels";
		break;
	case ERR_BADCHANMASK:
		result << ":Bad Channel Mask";
		break;
	case ERR_CHANOPRIVSNEEDED:
		result << ":You're not channel operator";
		break;
	case ERR_UNKNOWNMODE:
		result << ":is unknown mode char to me";
		break;
	case ERR_NOSUCHCHANNEL:
		result << ":No such channel";
		break;
	case ERR_BADCHANNELKEY:
		result << ":Bad channel key";
		break;
	case ERR_USERONCHANNEL:
		result << ":is already on channel";
		break;
	}
	result << "\n";
	send(client.getFd(), result.str().c_str(), result.str().size(), 0);
}

bool Errors::commandFound(const std::string &command)
{
	return command == "PASS" || command == "JOIN"
		|| command == "NICK" || command == "PART"
		|| command == "PING" || command == "PRIVMSG"
		|| command == "USER" || command == "MODE"
		|| command == "QUIT";
}

bool Errors::validParameters(Cmd &cmd, Client &client, Server &server)
{
	std::string command = cmd.getName();
	if (command == "PASS")
		return checkPASS(cmd, client, server);

	else if (command == "JOIN")
		return checkJOIN(cmd, client);

	else if (command == "NICK")
		return checkNICK(cmd, client, server);

	else if (command == "PART")
		return checkPART(cmd, client, server);

	else if (command == "PING")
		return checkPING(cmd, client);

	else if (command == "PRIVMSG")
		return checkPRIVMSG(cmd, client);

	else if (command == "USER")
		return checkUSER(cmd, client);

	else if (command == "KICK")
		return checkKICK(cmd, client);

	else if (command == "INVITE")
		return checkINVITE(cmd, client);

	else if (command == "TOPIC")
		return checkTOPIC(cmd, client);

	else if (command == "MODE")
		return checkMODE(cmd, client, server);
	return true;
}
