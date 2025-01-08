/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:04 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/07 16:51:53 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"
#include <ctime>

bool Errors::checkTOPIC(Cmd &cmd, Server& server, Client &client)
{
    std::string messageArgs[] = {client.getNickname(), "", ""};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);

	std::string channel_name = cmd.getParams()[0];
	std::map<std::string, Channel>::iterator it = server.getChannels().find(channel_name);
	messageArgs[1] = channel_name;
	if (it == server.getChannels().end())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);

	if (!client.isInsideTheChannel(channel_name))
	    return (Server::sendError(messageArgs, client.getFd(), ERR_NOTONCHANNEL), false);

	Channel& channel = it->second;

	if (cmd.getParams().size() == 1)
		return true;

	std::string topic = cmd.getParams()[1];
	if (!channel.isOperator(client.getFd()) && channel.hasTopicRestricions())
		return (messageArgs[0] = channel_name, Server::sendError(messageArgs, client.getFd(), ERR_CHANOPRIVSNEEDED), false);
	return true;
}

void Cmd::TOPIC(const Cmd& cmd, Server& server, Client& client) {

	std::string channel_name = cmd.getParams()[0];
	std::string messageArgs[] = {client.getNickname(), channel_name, "", ""};
	std::map<std::string, Channel>::iterator it = server.getChannels().find(channel_name);
	Channel& channel = it->second;

	if (cmd.getParams().size() == 1) {
		if (channel.getTopic().empty())
			return (Server::sendReply(messageArgs, client.getFd(), RPL_NOTOPIC), (void)0);
		messageArgs[2] = channel.getTopic();
		Server::sendReply(messageArgs, client.getFd(), RPL_TOPIC);
		messageArgs[2] = channel.getTopicModifier();
		std::stringstream time;
		time << channel.getTopicTime();
		messageArgs[3] = time.str();
		Server::sendReply(messageArgs, client.getFd(), RPL_TOPICWHOTIME);
		return ;
	}

	std::string topic = cmd.getParams()[1];
	if (topic.empty()) {
		if (channel.getTopic().empty())
			return ;
		channel.setTopic("");
		channel.setTopicModifier(client.getNickname());
		channel.setTopicTime(std::time(NULL));
		messageArgs[2] = channel.getTopic();
		Server::sendReply(messageArgs, client.getFd(), RPL_TOPIC);
		messageArgs[0] = channel_name;
		return (Server::sendReply(messageArgs, client.getFd(), RPL_NOTOPIC), (void)0);
	}
	if (channel.getTopic() == topic)
		return ;
	channel.setTopic(topic);
	channel.setTopicTime(std::time(NULL));
	channel.setTopicModifier(client.getNickname());
	messageArgs[1] = client.getUsername(); messageArgs[2] = channel_name; messageArgs[3] = topic;
	std::string message = RPL_TOPICADMIN(client.getNickname(), client.getUsername(), channel_name, topic);
	channel.broadcastMessage(message, -1);
}
