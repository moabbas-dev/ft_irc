/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:04 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/04 12:36:05 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkTOPIC(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	// TODO: Implement checkTOPIC command
	return true;
}

void Cmd::TOPIC(const Cmd& cmd, Server& server, Client& client) {
	(void)cmd;
	(void)server;
	(void)client;
	// TODO: Implement TOPIC command
}
