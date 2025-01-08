/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:45:21 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/08 21:24:11 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/Errors.hpp"
#include "../includes/Cmd.hpp"
#include "../includes/Client.hpp"

std::map<int, std::string> Errors::errors;


bool Errors::commandFound(const std::string &command)
{
	return command == "PASS"   || command == "JOIN"
		|| command == "NICK"   || command == "PART"
		|| command == "PING"   || command == "PRIVMSG"
		|| command == "USER"   || command == "MODE"
		|| command == "QUIT"   || command == "TOPIC"
		|| command == "INVITE" || command == "KICK";
}

bool Errors::validParameters(Cmd &cmd, Client &client, Server &server)
{
	std::string command = cmd.getName();
	if (command == "PASS")
		return checkPASS(cmd, client, server);

	else if (command == "JOIN")
		return checkJOIN(cmd, client);

	else if (command == "NICK")
		return checkNICK(cmd, client, server);

	else if (command == "PART")
		return checkPART(cmd, client, server);

	else if (command == "PING")
		return checkPING(cmd, client);

	else if (command == "PRIVMSG")
		return checkPRIVMSG(cmd, client, server);

	else if (command == "USER")
		return checkUSER(cmd, client);

	else if (command == "KICK")
		return checkKICK(cmd, client, server);

	else if (command == "INVITE")
		return checkINVITE(cmd, client, server);

	else if (command == "TOPIC")
		return checkTOPIC(cmd, server, client);

	else if (command == "MODE")
		return checkMODE(cmd, client, server);
	return true;
}
