/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:06 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/31 18:24:00 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::USER(const Cmd& cmd, Server& server, Client& client) {
    (void)cmd;
    (void)server;
    (void)client;
    std::string username = cmd.getParams()[0];
    if (username.empty())
        username = client.getNickname();
    else if (username.length() > _USERLEN)
        username = username.substr(0, _USERLEN);

    std::string realname = cmd.getParams()[3];
	if (realname.empty())
        realname = client.getNickname();
	client.setIsAuthenticated(true);
	client.setHasSetUser(true);
    client.setUsername(username);
    client.setRealname(realname);

    std::ostringstream oss;
    oss << client.getNickname() << " has set his username to: " << client.getUsername() << " and realname to: " << client.getRealname() << ".";
    Server::printResponse(oss.str() , BLUE);
    std::string msg001 = ": 001 " + client.getNickname() + " : Welcome to the IRC server!\n";
    send(client.getFd(), msg001.c_str(), msg001.size(), 0);
}
