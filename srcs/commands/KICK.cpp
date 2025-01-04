/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:05:51 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/04 12:36:37 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkKICK(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

void Cmd::KICK(const Cmd& cmd, Server& server, Client& client) {
	(void)cmd;
	(void)server;
	(void)client;
	// TODO: Implement KICK command
}
