/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:51:14 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/16 13:37:40 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPASS(Cmd &cmd, Client &client, Server &server)
{
	std::string messageArgs[] = {client.getNickname()};
	if (client.getIsAuthenticated() || client.getHasSetUser() || client.getHasSetNickName() || client.getHasSetPassword())
		return (Server::sendError(messageArgs, client.getFd(), ERR_ALREADYREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);

	if (cmd.getParams()[0] != server.getPassword())
		return (Server::sendError(messageArgs, client.getFd(), ERR_PASSWDMISMATCH), false);
	client.setHasSetPassword(true);
	return true;
}

void Cmd::PASS(const Cmd& cmd, Server& server, Client& client) {
    (void)cmd;
    (void)server;
    client.setHasSetPassword(true);
    std::string clientName = client.getoriginalhostname();
    Server::printResponse(clientName + " has registered.", BLUE);
}