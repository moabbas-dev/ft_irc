/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:45:21 by moabbas           #+#    #+#             */
/*   Updated: 2024/12/30 21:09:12 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Errors.hpp"
#include "../includes/Cmd.hpp"

std::map<int, std::string> Errors::errors;

static bool checkCorrectNickName(const std::string &nick)
{
    if (nick.empty())
        return false;

    if (std::isdigit(nick[0]))
        return false;

    for (size_t i = 0; i < nick.size(); ++i)
    {
        char c = nick[i];
        if (!(std::isalnum(c) || c == '[' || c == ']' || c == '{' || c == '}' || c == '\\' || c == '|'))
            return false;
    }

    return true;
}

bool Errors::checkPASS(Cmd &cmd, Client &client, Server &server)
{
	if (client.getIsAuthenticated() || client.getHasSetUser()
		|| client.getHasSetNickName() || client.getHasSetPassword())
	{
		raise(client.getNickname(), "You may not reregister", ERR_ALREADYREGISTERED);
		return false;
	}
	if (cmd.getParams().size() < 1)
	{
		raise(client.getNickname(), "Not enough parameters", ERR_NEEDMOREPARAMS);
		return false;
	}
	if (cmd.getParams()[0] != server.getPassword())
	{
		raise(client.getNickname(), "Password incorrect", ERR_PASSWDMISMATCH);
		return false;
	}
	client.setHasSetPassword(true);
	return true;
}

bool Errors::checkNICK(Cmd &cmd, Client &client, Server &server)
{
	if (!client.getHasSetPassword())
	{
		raise(client.getNickname(), "You have not registered", ERR_NOTREGISTERED);
		return false;
	}
	if (cmd.getParams().size() < 1)
	{
		raise(client.getNickname(), "No nickname given", ERR_NONICKNAMEGIVEN);
		return false;
	}
	if (!checkCorrectNickName(cmd.getParams()[0]))
	{
		raise(client.getNickname(), "Erroneus nickname", ERR_ERRONEUSNICKNAME);
		return false;
	}
	const std::map<int, Client> &clients = server.getClients();
	std::map<int, Client>::const_iterator it = clients.begin();
	while (it != clients.end())
	{
		if (it->first != client.getFd() && it->second.getNickname() == cmd.getParams()[0])
		{
			raise(client.getNickname(), "Nickname is already in use", ERR_NICKNAMEINUSE);
			return false;
		}
		++it;
	}
	client.setHasSetNickName(true);
	client.setNickname(cmd.getParams()[0]);
	return true;
}

bool Errors::checkJOIN(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkPART(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkPING(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkPRIVMSG(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkUSER(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkKICK(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkINVITE(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkTOPIC(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

bool Errors::checkMODE(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

void Errors::raise(const std::string &clientName, const std::string &msgName, int errorCode) 
{
	std::string result = msgName.empty()? clientName + " " : clientName + " " + msgName+ " ";
	switch(errorCode)
	{
		case ERR_UNKNOWCOMMAND:
			result.append(":Unknow Command");
			break;
		case ERR_NEEDMOREPARAMS:
			result.append(":Not enough parameters");
			break;
		case ERR_TOOMANYPARAMS:
			result.append(":Too many parameters");
			break;
		case ERR_PASSWDMISMATCH:
			result.append(":Password incorrect");
			break;
		case ERR_ALREADYREGISTERED:
			result.append(":You may not reregister");
			break;
		case ERR_NOTREGISTERED:
			result.append(":You have not registered");
			break;
		case ERR_NONICKNAMEGIVEN:
			result.append(":No nickname given");
			break;
		case ERR_ERRONEUSNICKNAME:
			result.append(":Erroneus nickname");
			break;
		case ERR_NICKNAMEINUSE:
			result.append("Nickname is already in use");
			break;
	}
}

bool Errors::commandFound(const std::string &command)
{
    return command == "PASS" || command == "JOIN"
        || command == "NICK" || command == "PART"
        || command == "PING" || command == "PRIVMSG"
        || command == "USER";
}

bool Errors::validParameters(Cmd &cmd, Client& client, Server &server)
{
    if (cmd.getName() == "PASS")
        return checkPASS(cmd, client, server);
    else if (cmd.getName() == "JOIN")
        return checkJOIN(cmd, client);
    else if (cmd.getName() == "NICK")
        return checkNICK(cmd, client, server);
    else if (cmd.getName() == "PART")
        return checkPART(cmd, client);
    else if (cmd.getName() == "PING")
        return checkPING(cmd, client);
    else if (cmd.getName() == "PRIVMSG")
        return checkPRIVMSG(cmd, client);
    else if (cmd.getName() == "USER")
        return checkUSER(cmd, client);
    else if (cmd.getName() == "KICK")
		return checkKICK(cmd, client);
	else if (cmd.getName() == "INVITE")
		return checkINVITE(cmd, client);
	else if (cmd.getName() == "TOPIC")
		return checkTOPIC(cmd, client);
	else if (cmd.getName() == "MODE")
		return checkMODE(cmd, client);
    return true;
}
