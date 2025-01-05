/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:49:35 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/05 18:35:12 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPING(Cmd &cmd, Client &client)
{
	std::string messageArgs[] = {client.getNickname()};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().empty())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
	return true;
}

void Cmd::PING(const Cmd &cmd, Server &server, Client &client)
{
    (void)server;
    std::ostringstream oss;
    oss <<" PONG " << cmd.getParams()[0] << std::endl;

	std::string message = oss.str();
	send(client.getFd(), message.c_str(), message.size(), 0);
}
