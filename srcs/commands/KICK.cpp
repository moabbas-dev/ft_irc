/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:05:51 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/08 02:10:25 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkKICK(Cmd &cmd, Client &client, Server &server)
{
	std::string messageArgs[] = {client.getNickname(), "", ""};
    if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

    if (cmd.getParams().size() < 2)
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
	
	messageArgs[0] = client.getUsername();
	messageArgs[1] = cmd.getParams()[0];
	if (!server.channelExistInServer(cmd.getParams()[0]))
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);

	messageArgs[0] = cmd.getParams()[0];
	if (!client.isOperatorInChannel(cmd.getParams()[0], server))
		return (Server::sendError(messageArgs, client.getFd(), ERR_CHANOPRIVSNEEDED), false);

	// messageArgs[0] = client.getNickname();
	// if (!client.isInsideTheChannel(cmd.getParams()[0]))
	// 	return (Server::sendError(messageArgs, client.getFd(), ERR_NOTONCHANNEL), false);

    return true;
}

void Cmd::KICK(const Cmd& cmd, Server& server, Client& client)
{
    const std::vector<std::string>& params = cmd.getParams();
    const std::string& channelName = params[0];
    const std::vector<std::string> users = split(params[1], ',');
    const std::string comment = (params.size() > 2) ? params[2] : "Kicked by operator";



    // for (const std::string& user : users) {
    //     Client* targetClient = server.getClientByNickname(user);

    //     if (!targetClient || !channel->hasClient(user)) {
    //         Server::sendError({client.getNickname(), user, channelName}, client.getFd(), ERR_USERNOTINCHANNEL);
    //         continue;
    //     }

    //     // Remove the user and notify the channel
    //     channel->removeClient(user);
    //     std::string kickMessage = ":" + client.getNickname() + " KICK " + channelName + " " + user + " :" + comment;
    //     channel->broadcastMessage(kickMessage, client.getFd());
    // }
}



