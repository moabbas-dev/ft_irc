/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:45:21 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/03 14:48:13 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>

#include "../includes/Errors.hpp"
#include "../includes/Cmd.hpp"
#include "../includes/Client.hpp"

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
		return (raise(client, "", ERR_ALREADYREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (raise(client, "", ERR_NEEDMOREPARAMS), false);

	if (cmd.getParams()[0] != server.getPassword())
		return (raise(client, "", ERR_PASSWDMISMATCH), false);
	client.setHasSetPassword(true);
	return true;
}

bool Errors::checkNICK(Cmd &cmd, Client &client, Server &server)
{
	if (!client.getHasSetPassword())
		return (raise(client, "", ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (raise(client, "", ERR_NONICKNAMEGIVEN), false);

	if (!checkCorrectNickName(cmd.getParams()[0]))
		return (raise(client, "", ERR_ERRONEUSNICKNAME), false);

	const std::map<int, Client> &clients = server.getClients();
	std::map<int, Client>::const_iterator it = clients.begin();
	while (it != clients.end())
	{
		if (it->first != client.getFd() && it->second.getNickname() == cmd.getParams()[0])
			return (raise(client, cmd.getParams()[0], ERR_NICKNAMEINUSE), false);
		++it;
	}
	client.setHasSetNickName(true);
	return true;
}


bool Errors::checkPART(Cmd &cmd, Client &client, Server &server)
{
	// (void)cmd;
	// (void)client;
	if (cmd.getParams().size() < 1)
		return (raise(client, "", ERR_NEEDMOREPARAMS), false);

	std::vector<std::string> splitted_params = split(cmd.getParams()[0], ',');
	std::vector<std::string>::iterator it = splitted_params.begin();

	while (it != splitted_params.end())
	{
		if (std::find(client.getChannels().begin(), client.getChannels().end(), *it) == client.getChannels().end())
			return (raise(client, *it, ERR_NOTONCHANNEL), false);

		if (std::find(server.getChannels().begin(), server.getChannels().end(), *it) == server.getChannels().end())
			return (raise(client, *it, ERR_NOSUCHCHANNEL), false);

		++it;
	}

	return true;
}

bool Errors::checkPING(Cmd &cmd, Client &client)
{
	if (!client.getIsAuthenticated())
		return (raise(client, cmd.getName(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().empty())
		return (raise(client, cmd.getName(), ERR_NEEDMOREPARAMS), false);
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
	if(!client.getHasSetPassword() || !client.getHasSetNickName())
		return (raise(client, cmd.getName(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 4)
		return (raise(client, cmd.getName(), ERR_NEEDMOREPARAMS), false);

	if (cmd.getParams().size() > 4)
		return (raise(client, cmd.getName(), ERR_TOOMANYPARAMS), false);
// a cmd like this PING LOL
// should return on the client terminal like this 
// 
	if (client.getIsAuthenticated()) 
		return (raise(client, cmd.getName(), ERR_ALREADYREGISTERED), false);

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

void Errors::raise(Client& client, const std::string &msgName, int errorCode)
{
	std::string clientName = client.getHasSetNickName()? client.getNickname() : client.getHostName();
	std::string result = msgName.empty()? clientName + " " : clientName + " " + msgName+ " ";
	switch(errorCode)
	{
		case ERR_UNKNOWCOMMAND:
			result.append(":Unknown Command");
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
			result.append(":Nickname is already in use");
			break;
		case ERR_TOOMANYCHANNELS:
			result.append(":You have joined too many channels");
			break;
		case ERR_BADCHANMASK:
			result.append(":Bad Channel Mask");
			break;
		case ERR_CHANOPRIVSNEEDED:
			result.append(":You're not channel operator");
			break;
		case ERR_UNKNOWNMODE:
			result.append(":is unknown mode char to me");
			break;
		case ERR_NOSUCHCHANNEL:
			result.append(":No such channel");
			break;
		case ERR_BADCHANNELKEY:
			result.append(":Bad channel key");
			break;
	}
	result.append("\n");
	send(client.getFd(), result.c_str(), result.size(), 0);
}

bool Errors::commandFound(const std::string &command)
{
    return command == "PASS" || command == "JOIN"
        || command == "NICK" || command == "PART"
        || command == "PING" || command == "PRIVMSG"
        || command == "USER" || command == "MODE";
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
        return checkPART(cmd, client, server);
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
		return checkMODE(cmd, client, server);
    return true;
}
