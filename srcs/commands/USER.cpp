/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:06 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/17 14:53:53 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkUSER(Cmd &cmd, Client &client)
{
    std::string messageArgs[] = { cmd.getParams()[0] };
	if (!client.getHasSetPassword() || !client.getHasSetNickName())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().size() < 4)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);


	if (client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_ALREADYREGISTERED), false);

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
    std::string message[] = {client.getNickname()};
    Server::sendReply(message, client.getFd(), RPL_WELCOME);
}
