/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:48:58 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/08 22:15:33 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPRIVMSG(Cmd &cmd, Client &client, Server &server)
{
    std::string messageArgs[] = {client.getNickname(), "", ""};
    bool isAchannel;

    if (!client.getIsAuthenticated())
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);
    if (cmd.getParams().size() < 2)
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
    isAchannel = cmd.getParams()[0][0] == '#';
    if (isAchannel)
    {
        if (!server.channelExistInServer(cmd.getParams()[0]))
        {
            messageArgs[0] = client.getUsername();
            messageArgs[1] = cmd.getParams()[0];
            return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);
        }
        if (!client.isInsideTheChannel(cmd.getParams()[0]))
        {
            messageArgs[0] = client.getUsername();
            messageArgs[1] = cmd.getParams()[0];
            return (Server::sendError(messageArgs, client.getFd(), ERR_CANNOTSENDTOCHAN), false);
        }
    }
    else
    {
        if (!server.clientIsInServer(cmd.getParams()[0]))
        {
            messageArgs[0] = client.getUsername();
            messageArgs[1] = cmd.getParams()[0];
            return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);
        }
    }
    if (cmd.getParams()[1].empty())
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTEXTTOSEND), false);
    return true;
}

void Cmd::PRIVMSG(const Cmd &cmd, Server &server, Client &client)
{
    std::cout << "Executing PRIVMSG command\n";
    (void)cmd;
    (void)server;
    (void)client;
    // @mabbas && @jfatfat > here we can  implement PRIVMSG
}