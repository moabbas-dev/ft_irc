/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:01 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/08 15:38:34 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkINVITE(Cmd &cmd, Client &client, Server &server)
{
	std::string messageArgs[] = {client.getNickname(), "", ""};
	
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);
	if (cmd.getParams().size() < 2)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
	if (!server.channelExistInServer(cmd.getParams()[1]))
	{
		messageArgs[0] = client.getUsername();
		messageArgs[1] = cmd.getParams()[0];
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);
	}
	if (!client.isInsideTheChannel(cmd.getParams()[1]))
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTONCHANNEL), false);
	messageArgs[0] = client.getNickname();
	if (!client.isOperatorInChannel(cmd.getParams()[1], server))
		return (Server::sendError(messageArgs, client.getFd(), ERR_CHANOPRIVSNEEDED), false);
	messageArgs[0] = client.getUsername();
	if (!server.clientIsInServer(cmd.getParams()[0]))
	{
		messageArgs[0] = cmd.getParams()[1];
		messageArgs[1] = cmd.getParams()[0];
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHNICK), false);
	}
	Channel *channel = server.getSpecifiedChannel(cmd.getParams()[1]);
	if (!channel)
		return false;
	if (channel->clientIsInChannel(cmd.getParams()[0]))
	{
		messageArgs[0] = cmd.getParams()[0];
		messageArgs[1] = cmd.getParams()[1];
		return (Server::sendError(messageArgs, client.getFd(), ERR_USERONCHANNEL), false);
	}
	if (channel->getHasUserLimit() && channel->getClientsNumber() == channel->getUserLimit())
	{
		messageArgs[0] = client.getNickname();
		messageArgs[1] = cmd.getParams()[0];
		messageArgs[2] = cmd.getParams()[1];
		return (Server::sendError(messageArgs, client.getFd(), ERR_CHANNELISFULL), false);
	}
	return true;
}

void Cmd::INVITE(const Cmd& cmd, Server& server, Client& client) {
	Client *clt = server.getSpecifiedClient(cmd.getParams()[0]);
	if (!clt)
	{
		std::string messageArgs[] = {client.getNickname(), cmd.getParams()[0], ""};
		Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHNICK);
		return ;
	}
	
	std::string messageArgs[] = {client.getNickname(), clt->getNickname(), cmd.getParams()[1]};
	Server::sendReply(messageArgs, client.getFd(), RPL_INVITING);
	clt->addInvitationToChannel(cmd.getParams()[1]);
	std::string cltMsg = ":" + client.getHostName() + " INVITE " + clt->getNickname() + " " + cmd.getParams()[1] + "\r\n";
	send(clt->getFd(), cltMsg.c_str(), cltMsg.size(), 0);
}
