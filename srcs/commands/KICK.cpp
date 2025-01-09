/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:05:51 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/09 12:18:25 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkKICK(Cmd &cmd, Client &client, Server& server)
{
    std::string messageArgs[] = {client.getNickname(), ""};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 2)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
	
	std::string channel_name = cmd.getParams()[0];
	messageArgs[1] = channel_name;
	if (server.getChannels().find(channel_name) == server.getChannels().end())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);

	if (!client.isInsideTheChannel(channel_name)) 
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTONTHATCHANNEL), false);
	
	messageArgs[0] = channel_name;
	if (!server.getChannels().find(channel_name)->second.isOperator(client.getFd())) 
		return (Server::sendError(messageArgs, client.getFd(), ERR_CHANOPRIVSNEEDED), false);

	std::vector<std::string> clients_names = split(cmd.getParams()[1], ',');
	std::vector<std::string> tmp_kick_users = client.getTempKickUsers();
	for(size_t i = 0;i < clients_names.size();i++) {
		if (!server.getSpecifiedClient(clients_names[i])) {
			std::cout << clients_names[i] << " doesn't exist";
			messageArgs[0] = client.getNickname(); messageArgs[1] = channel_name;
			Server::sendError(messageArgs, client.getFd(), ERR_NOTONTHATCHANNEL);
			continue;
		} else if (!server.getSpecifiedChannel(channel_name)->isClientInChannel(server.getSpecifiedClient(clients_names[i])->getFd())) {
			messageArgs[0] = clients_names[i]; messageArgs[1] = channel_name;
			Server::sendError(messageArgs, client.getFd(), ERR_NOTONTHATCHANNEL);
			continue;
		}
		tmp_kick_users.push_back(clients_names[i]);
	}
	client.setTempKickUsers(tmp_kick_users);
	clients_names.clear();
	return true;
}

void Cmd::KICK(const Cmd& cmd, Server& server, Client& client) {
	std::string channel_name = cmd.getParams()[0];
	std::vector<std::string> tmp_users = client.getTempKickUsers();
	std::string comment = cmd.getParams().size() >= 3? cmd.getParams()[2] : "Mr. Walid please don't give us zero :)";

	Channel& target_channel = server.getChannels().find(channel_name)->second;
	for (size_t i = 0;i < tmp_users.size();i++) {
		Client *target_client = server.getSpecifiedClient(tmp_users[i]);
		std::string reply = RPL_KICK(client.getNickname(), client.getUsername(), channel_name, target_client->getNickname(), comment);
		target_channel.broadcastMessage(reply, -1);
		if (target_channel.isOperator(target_client->getFd()))
			target_channel.removeClient(target_client->getFd());
		target_channel.removeClient(target_client->getFd());
		target_client->removeChannel(target_channel);
	}
	if (target_channel.isEmpty())
		server.deleteChannel(channel_name);
	client.clearTempKickUsers();
}



