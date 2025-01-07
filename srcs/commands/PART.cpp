/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:48:10 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/07 23:38:54 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPART(Cmd &cmd, Client &client, Server &server)
{
	std::string messageArgs[] = {client.getNickname(), ""};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);

	std::vector<std::string> splitted_params = split(cmd.getParams()[0], ',');
	std::vector<std::string>::iterator it = splitted_params.begin();
	std::vector<Channel> clientChannels = client.getChannels();
	std::map<std::string, Channel> servChannels = server.getChannels();
    bool noChannelFound = false;
    while (it != splitted_params.end())
    {
        bool foundInClientChannels = false;
        for (std::vector<Channel>::iterator itChannel = clientChannels.begin();
             itChannel != clientChannels.end();
             ++itChannel)
        {
            if (itChannel->getName() == *it)
            {
                foundInClientChannels = true;
                noChannelFound = true;
                break;
            }
        }
        std::string messageArgs[] = {client.getNickname(), *it};
        if (servChannels.find(*it) == servChannels.end())
            Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL);
        else if (!foundInClientChannels)
            Server::sendError(messageArgs, client.getFd(), ERR_NOTONCHANNEL);
        ++it;
    }

	return noChannelFound;
}

void Cmd::PART(const Cmd& cmd, Server& server, Client& client) {

    std::vector<std::string> splitted_params = split(cmd.getParams()[0], ',');
    std::vector<std::string>::iterator it = splitted_params.begin();

    while (it != splitted_params.end()) {
        std::string channel_name = *it;
        // Channel* channel = server.findChannel(channel_name);
        std::map<std::string, Channel>::iterator channel_iterator = server.getChannels().find(channel_name);
        if (channel_iterator == server.getChannels().end()) {
            std::string messageArgs[] = {client.getNickname(), *it};
            Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL);
            return ;
        }
        Channel& channel = channel_iterator->second;
        channel.removeClient(client.getFd());
        client.removeChannel(channel);
        std::string reason = (cmd.getParams().size() > 1) ? cmd.getParams()[1] : "";
        std::string part_message = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost" + " PART " + channel_name;
        if (!reason.empty())
            part_message += reason[0] == ':' ? reason : " :" + reason;
        part_message += "\r\n";
        send(client.getFd(), part_message.c_str(), part_message.size(), 0);
        channel.broadcastMessage(part_message, client.getFd());
        if (channel.isEmpty())
            server.deleteChannel(channel_name);
        ++it;
    }
}
