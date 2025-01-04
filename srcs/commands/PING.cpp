/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:49:35 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/04 12:36:17 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPING(Cmd &cmd, Client &client)
{
	if (!client.getIsAuthenticated())
		return (raise(client, cmd.getName(), ERR_NOTREGISTERED), false);

	if (cmd.getParams().empty())
		return (raise(client, cmd.getName(), ERR_NEEDMOREPARAMS), false);
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
