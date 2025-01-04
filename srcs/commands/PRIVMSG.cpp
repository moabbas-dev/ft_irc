/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:48:58 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/04 12:36:13 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cmd.hpp"
#include "../../includes/Errors.hpp"

bool Errors::checkPRIVMSG(Cmd &cmd, Client &client)
{
	(void)cmd;
	(void)client;
	return true;
}

void Cmd::PRIVMSG(const Cmd& cmd, Server& server, Client& client) {
    std::cout << "Executing PRIVMSG command\n";
    (void)cmd;
    (void)server;
    (void)client;
    // @mabbas && @jfatfat > here we can  implement PRIVMSG
}