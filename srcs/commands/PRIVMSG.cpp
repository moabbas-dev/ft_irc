/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:48:58 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/12 11:46:19 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

void askMarvin(Bot* bot, Client& client, std::string command) {
    if (command == "!age")
        bot->sendAgeMsg(client);
    else if (command == "!joke")
        bot->sendJokeMsg(client);
    else if(command == "!help")
        bot->sendHelpMsg(client);
    else 
        bot->sendUnknowCmdMsg(client);
}


bool Errors::checkPRIVMSG(Cmd &cmd, Client &client, Server &server)
{
    std::string messageArgs[] = {client.getNickname(), "", ""};

    if (!client.getIsAuthenticated())
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

    if (cmd.getParams().size() < 2)
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);

    std::vector<std::string> users = split(cmd.getParams()[0], ',');
    std::vector<std::string> tmp_users = client.getTempKickUsers();
    for (size_t i = 0;i < users.size(); i++) {
        if (users[i][0] == '#') {
            if (!server.channelExistInServer(users[i])) {
                messageArgs[0] = client.getUsername();
                messageArgs[1] = users[i];
                return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);
            }
            if (!client.isInsideTheChannel(users[i])) {
                messageArgs[0] = client.getUsername();
                messageArgs[1] = users[i];
                return (Server::sendError(messageArgs, client.getFd(), ERR_CANNOTSENDTOCHAN), false);
            }
            tmp_users.push_back(users[i]);
        } else {
            if (!server.clientIsInServer(users[i]))
            {
                messageArgs[0] = client.getUsername();
                messageArgs[1] = users[i];
                return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHNICK), false);
            }
            tmp_users.push_back(users[i]);
        }
    }
    if (cmd.getParams()[1].empty())
        return (Server::sendError(messageArgs, client.getFd(), ERR_NOTEXTTOSEND), false);
    client.setTempKickUsers(tmp_users);
    return true;
}

void Cmd::PRIVMSG(const Cmd &cmd, Server &server, Client &client)
{
    std::vector<std::string> users = client.getTempKickUsers();
    for (size_t i = 0;i < users.size(); i++) {
        if (users[i][0] == '#') {
            Channel *channel = server.getSpecifiedChannel(users[i]);
            std::string msg = ":" + client.getNickname() + "!~"
                    + client.getUsername() + "@localhost PRIVMSG "
                    + channel->getName() + " :" + cmd.getParams()[1] + "\r\n";
            channel->broadcastMessage(msg, client.getFd());
        } else {
            Client *clt = server.getSpecifiedClient(users[i]);
            std::string msg;
            if (clt->getFd() == 5) {
                Bot* bot = server.getBot();
                askMarvin(bot, client, cmd.getParams()[1]);
            }
            else {
                msg = ":" + client.getNickname() + "!~"
                        + client.getUsername() + "@localhost PRIVMSG "
                        + clt->getNickname() + " :" + cmd.getParams()[1] + "\r\n";
                if (client.getNickname() != clt->getNickname())
                    send(clt->getFd(), msg.c_str(), msg.size(), 0);
            }
        }
    }
    users.clear();
    client.setTempKickUsers(users);
}
