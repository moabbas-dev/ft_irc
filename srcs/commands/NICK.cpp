/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:34 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/05 18:49:24 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

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

bool Errors::checkNICK(Cmd &cmd, Client &client, Server &server)
{
	std::string messageArgs[] = {client.getNickname()};

	if (!client.getHasSetPassword())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NONICKNAMEGIVEN), false);

	if (!checkCorrectNickName(cmd.getParams()[0]))
		return (Server::sendError(messageArgs, client.getFd(), ERR_ERRONEUSNICK), false);

	const std::map<int, Client> &clients = server.getClients();
	std::map<int, Client>::const_iterator it = clients.begin();
	while (it != clients.end())
	{
		if (it->first != client.getFd() && it->second.getNickname() == cmd.getParams()[0])
			return (Server::sendError(messageArgs, client.getFd(), ERR_NICKINUSE), false);
		++it;
	}
	client.setHasSetNickName(true);
	return true;
}

void Cmd::NICK(const Cmd& cmd, Server& server, Client& client) {
    (void)server;
    
    std::ostringstream oss;
    if (!client.getHasSetNickName()) {
        client.setNickname(cmd.getParams()[0]);
        oss << client.getHostName() << "<" << client.getFd()
            << "> has set his Nickname to: "<< client.getNickname() << ".";
    }
    else {
        std::string oldNickname = client.getNickname();
        client.setNickname(cmd.getParams()[0]);
        oss << oldNickname << " changed his nickname to: " << client.getNickname() << ".";
    }
    Server::printResponse(oss.str() , BLUE);
    client.setHasSetNickName(true);
}