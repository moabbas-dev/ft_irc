/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:34 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/29 13:37:20 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::NICK(const Cmd& cmd, Server& server, Client& client) {
    (void)server;
    if (cmd.getParams().size() != 1) {
        Cmd::errorServerClient("Error: Cannot resolve NICK command",
        "server: Error when setting nickname (NICK is invalid)", client.getFd());
    }
    else {
    client.setNickname(cmd.getParams()[0]);
    std::cout << "NickName for user <" << client.getFd() << "> has been set" << std::endl;
    }
}
