/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:51:14 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/04 12:36:21 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPASS(Cmd &cmd, Client &client, Server &server)
{
	if (client.getIsAuthenticated() || client.getHasSetUser() || client.getHasSetNickName() || client.getHasSetPassword())
		return (raise(client, "", ERR_ALREADYREGISTERED), false);

	if (cmd.getParams().size() < 1)
		return (raise(client, "", ERR_NEEDMOREPARAMS), false);

	if (cmd.getParams()[0] != server.getPassword())
		return (raise(client, "", ERR_PASSWDMISMATCH), false);
	client.setHasSetPassword(true);
	return true;
}

void Cmd::PASS(const Cmd& cmd, Server& server, Client& client) {
    (void)cmd;
    (void)server;
    client.setHasSetPassword(true);
    std::string clientName = client.getHasSetNickName()? client.getNickname()
                            : client.getHasSetUser()? client.getUsername()
                            : client.getHostName();
    Server::printResponse(clientName + " has registered.", BLUE);
}