/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:51:14 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/31 13:57:42 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::PASS(const Cmd& cmd, Server& server, Client& client) {
    (void)cmd;
    (void)server;
    client.setHasSetPassword(true);
    std::string clientName = client.getHasSetNickName()? client.getNickname()
                            : client.getHasSetUser()? client.getUsername()
                            : client.getHostName();
    Server::printResponse(clientName + " has registered.", BLUE);
}