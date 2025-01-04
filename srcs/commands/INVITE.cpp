/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:01 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/04 12:35:54 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkINVITE(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

void Cmd::INVITE(const Cmd& cmd, Server& server, Client& client) {
	(void)cmd;
	(void)server;
	(void)client;
	// TODO: Implement INVITE command
}
