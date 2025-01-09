/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:34 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/09 20:31:05 by moabbas          ###   ########.fr       */
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

void updateOldNickInChannels(Client& client,Server& server) {
	std::vector<Channel> &client_channels = client.getChannels();
	std::map<std::string, Channel> &server_channels = server.getChannels();
	for (int i = 0;i < (int)client_channels.size();i++) {
		std::vector<Client> &channel_clients = client_channels[i].getClients();
		for (size_t j = 0;j < channel_clients.size();j++) {
			if (channel_clients[j].getFd() == client.getFd())
				channel_clients[j].setNickname(client.getNickname());
		}
		std::map<std::string, Channel>::iterator channel_it = server_channels.find(client_channels[i].getName());
		if (channel_it != server_channels.end()) {
			std::vector<Client> &channel_it_clients = channel_it->second.getClients();
			for (size_t j = 0;j < channel_it_clients.size();j++) {
				if (channel_it_clients[j].getFd() == client.getFd())
					channel_it_clients[j].setNickname(client.getNickname());
			}
		}
	}
}

void Cmd::NICK(const Cmd& cmd, Server& server, Client& client) {
    (void)server;
    
    std::ostringstream oss;
    if (client.getNickname().empty()) {
        client.setNickname(cmd.getParams()[0]);
        oss << client.getHostName() << "<" << client.getFd()
            << "> has set his Nickname to: "<< client.getNickname() << ".";
    }
    else {
        std::string oldNickname = client.getNickname().empty()? client.getoriginalhostname() : client.getNickname();
		Client* oldClient = server.getSpecifiedClient(client.getNickname());
        client.setNickname(cmd.getParams()[0]);
		oldClient->setNickname(cmd.getParams()[0]);
		std::map<int, Client> &server_clients = server.getClients();
		server_clients[client.getFd()] = client;
		updateOldNickInChannels(client, server);
        oss << oldNickname << " changed his nickname to: " << client.getNickname() << ".";
		std::string message = RPL_NICKCHANGE(oldNickname, cmd.getParams()[0]);
		send(client.getFd(), message.c_str(), message.size(), 0);
		std::vector<Channel>& client_channels = client.getChannels();
		for (size_t i = 0;i < client_channels.size(); i++) {
			client_channels[i].broadcastMessage(message, client.getFd());
		}
    }
    Server::printResponse(oss.str() , BLUE);
    client.setHasSetNickName(true);
}
