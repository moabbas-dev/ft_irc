/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:34 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/31 14:48:34 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::NICK(const Cmd& cmd, Server& server, Client& client) {
    (void)server;
    
    std::ostringstream oss;
    if (!client.getHasSetNickName()) {
        client.setNickname(cmd.getParams()[0]);
        oss << client.getHostName() << "<" << client.getFd()
            << "> has set his Nickname to: "<< client.getNickname() << ".";
    }
    else {
        std::string oldNickname = client.getNickname();
        client.setNickname(cmd.getParams()[0]);
        oss << oldNickname << " changed his nickname to: " << client.getNickname() << ".";
    }
    Server::printResponse(oss.str() , BLUE);
    client.setHasSetNickName(true);
}
