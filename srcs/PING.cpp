/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:49:35 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/29 23:20:21 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::PING(const Cmd &cmd, Server &server, Client &client)
{
    std::cout << "Executing PING command\n";
    // (void)cmd;
    (void)server;
    // (void)client;
    if (cmd.getParams().empty() || !cmd.getParams().size())
        return (cmd.errorServerClient("409", "empty ....", client.getFd()));
    std::string	_string_to_send = ":" + client.getUsername() + " PONG :" + cmd.getParams()[0] + "\n";
    send(client.getFd(), _string_to_send.c_str(), _string_to_send.length(), 0);
}

// a cmd like this PING LOL
// should return on the client terminal like this 
// :afarachi!afarachi@localhost PONG :LOL