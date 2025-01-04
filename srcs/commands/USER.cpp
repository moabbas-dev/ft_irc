/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:06 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/04 15:09:25 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkUSER(Cmd &cmd, Client &client)
{
	if (!client.getHasSetPassword() || !client.getHasSetNickName())
		return (raise(client, cmd.getName(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 4)
		return (raise(client, cmd.getName(), ERR_NEEDMOREPARAMS), false);

	if (cmd.getParams().size() > 4)
		return (raise(client, cmd.getName(), ERR_TOOMANYPARAMS), false);

	if (client.getIsAuthenticated())
		return (raise(client, cmd.getName(), ERR_ALREADYREGISTERED), false);

	return true;
}

void Cmd::USER(const Cmd& cmd, Server& server, Client& client) {
    (void)cmd;
    (void)server;
    (void)client;
    std::string username = cmd.getParams()[0];
    if (username.empty())
        username = client.getNickname();
    else if (username.length() > _USERLEN)
        username = username.substr(0, _USERLEN);

    std::string realname = cmd.getParams()[3];
	if (realname.empty())
        realname = client.getNickname();
	client.setIsAuthenticated(true);
	client.setHasSetUser(true);
    client.setUsername(username);
    client.setRealname(realname);

    std::ostringstream oss;
    oss << client.getNickname() << " has set his username to: " << client.getUsername() << " and realname to: " << client.getRealname() << ".";
    Server::printResponse(oss.str() , BLUE);
    Server::sendReply(RPL_CONNECTED(client.getNickname()), client.getFd());
}
