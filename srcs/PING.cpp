/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:49:35 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/03 18:33:06 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::PING(const Cmd &cmd, Server &server, Client &client)
{
    (void)server;
    std::ostringstream oss;
    oss <<" PONG " << cmd.getParams()[0] << std::endl;

	std::string message = oss.str();
	send(client.getFd(), message.c_str(), message.size(), 0);
}

// a cmd like this PING LOL
// should return on the client terminal like this 
// :afarachi!afarachi@localhost PONG :LOL