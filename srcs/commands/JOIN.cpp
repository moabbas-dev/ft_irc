/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:46:43 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/05 18:16:26 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"
#include "../../includes/Channel.hpp"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

bool checkChannelName(std::string channelName) {
    for (size_t i = 1; i < channelName.length(); ++i) {
        char c = channelName[i];
        if (!std::isalnum(c) && c != ',' && c != '_' && c != '-' && c != '~' && c != '.' && c != '+' && c != '#' && c != '&')
            return false;
    }

	return channelName.length() > 3
		&& channelName.length() <= MAX_CHANNEL_NAME_LENGTH
		&& (channelName[0]== '&' || channelName[0] == '#');
}

bool checkChannelKey(std::string channelKey) {
	for (size_t i = 1; i < channelKey.length(); ++i) {
        char c = channelKey[i];
        if (std::isspace(c))
            return false;
    }
	return true;
}

bool Errors::checkJOIN(Cmd &cmd, Client &client)
{
    std::string messageArgs[] = {client.getNickname(), ""};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);

    if (cmd.getParams().size() == 1) {
        messageArgs[1] = cmd.getParams()[0];
        if (!cmd.getParams()[0].empty() && cmd.getParams()[0].at(0) == '&')
            return (Server::sendError(messageArgs, client.getFd(), ERR_BADCHANNELKEY), false);
        if (!checkChannelName(cmd.getParams()[0]))
			return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);
    }

	if (cmd.getParams().size() > 2)
		return (Server::sendError(messageArgs, client.getFd(), ERR_BADCHANNELKEY), false);

	std::vector<std::string> channelsNames = split(cmd.getParams()[0], ',');
	std::vector<std::string> channelsKeys = cmd.getParams().size() == 2? split(cmd.getParams()[1], ',') : std::vector<std::string>();
	std::vector<Channel> channels = client.getTempChannels();
	std::vector<std::string>::iterator key_it = channelsKeys.begin();
	for (std::vector<std::string>::iterator name_it = channelsNames.begin() ; name_it != channelsNames.end(); ++name_it) {
		if (!checkChannelName(*name_it)) {
            std::string messageArgs[] = {client.getNickname(), *name_it};
            Server::sendError(messageArgs, client.getFd(), ERR_BADCHANNELMASK);
        }
		else {
			if (key_it != channelsKeys.end()) {
				if (!checkChannelKey(*key_it)) {
                    std::string messageArgs[] = {client.getNickname(), *name_it};
					Server::sendError(messageArgs, client.getFd(), ERR_BADCHANNELKEY);
                }
				else
					channels.push_back(Channel(*name_it, *key_it));
				++key_it;
			}
			else
				channels.push_back(Channel(*name_it));
		}
	}
	for (std::vector<Channel>::iterator channel = channels.begin() ; channel != channels.end();) {
            std::string messageArgs2[] = {client.getNickname(), (*channel).getName()};
        if ((*channel).getName().at(0) == '#' && (*channel).hasKey()) {
            Server::sendError(messageArgs2, client.getFd(), ERR_NOSUCHCHANNEL);
            channel = channels.erase(channel);
        }
        else if ((*channel).getName().at(0) == '&' && !(*channel).hasKey()) {
            Server::sendError(messageArgs2, client.getFd(), ERR_BADCHANNELKEY);
            channel = channels.erase(channel);
        } else
            channel++;
    }
	client.setTempChannels(channels);
    channelsNames.clear();
    channelsKeys.clear();
	return true;
}

bool alreadyInchannel (const std::vector<Channel>& client_channels, const Channel& channel) {
    std::vector<Channel>::const_iterator it = client_channels.begin();
    while (it != client_channels.end())
    {
        if (*it == channel)
            return true;
        ++it;
    }
    return false;
}

void Cmd::JOIN(const Cmd& cmd, Server& server, Client& client) {
    (void)cmd;
    std::vector<Channel> tmp_channels = client.getTempChannels();
    std::vector<Channel>& client_channels = client.getChannels();
    std::map<std::string, Channel>& server_channels = server.getChannels();

    for (std::vector<Channel>::iterator it = tmp_channels.begin(); it != tmp_channels.end(); ++it) {
        const std::string& channel_name = it->getName();
        const std::string& channel_key = it->getChannelKey();

        std::map<std::string, Channel>::iterator channel_it = server_channels.find(channel_name);
        if (channel_it != server_channels.end()) {
            Channel& server_channel = channel_it->second;

            if (server_channel.hasKey() && server_channel.getChannelKey() != channel_key) {
                std::string messageArgs[] = {client.getNickname(), channel_name};
                Server::sendError(messageArgs, client.getFd(), ERR_BADCHANNELKEY);
                continue;
            } else if (alreadyInchannel(client_channels, server_channel)) {
                std::string messageArgs[] = {client.getNickname(), channel_name};
                Server::sendError(messageArgs, client.getFd(), ERR_USERONCHANNEL);
                continue;
            }
            server_channel.addClient(client);
            client_channels.push_back(server_channel);
            std::string message = (client.getHasSetNickName()?client.getNickname() : client.getHostName()) + " has joined channel " + channel_name + ".";
            Server::printResponse(message, BLUE);
            std::string messageArgs[] = {client.getNickname(), client.getUsername(),client.getIPadd(),server_channel.getName(), server_channel.clientslist()};
            Server::sendReply(messageArgs, client.getFd(), RPL_CREATECHANNELMSG);
            server_channel.broadcastMessage(RPL_JOINMSG(client.getNickname(), client.getUsername(),client.getIPadd(),server_channel.getName()), client.getFd());
        } else {
            Channel new_channel(channel_name, channel_key);
            new_channel.addClient(client);
            new_channel.addOperator(client.getFd());
            server_channels[channel_name] = new_channel;
            client_channels.push_back(new_channel);
            std::string message = (client.getHasSetNickName() ? client.getNickname() : client.getHostName())
                + " has created channel " + channel_name  + (new_channel.hasKey()? " with key=" + new_channel.getChannelKey() : "") + ".";
            Server::printResponse(message, GREEN);
            std::string messageArgs[] = {client.getNickname(), client.getUsername(),client.getIPadd(),new_channel.getName(), new_channel.clientslist()};
            Server::sendReply(messageArgs, client.getFd(), RPL_CREATECHANNELMSG);
        }
    }
    client.clearTempChannels();
}