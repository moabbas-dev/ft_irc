/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:39:19 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/03 20:02:37 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Errors.hpp"
#include "../includes/Cmd.hpp"

// bool Errors::checkQUIT(Cmd &cmd, Client &client, Server &server) {
// 	(void)cmd;

// 	return true;
// }

void Cmd::QUIT(const Cmd& cmd, Server& server, Client& client) {
	(void)cmd;
	std::ostringstream oss;
	std::string clientName = client.getHasSetNickName()? client.getNickname() : client.getHostName();
	oss << clientName << "<" << client.getFd() << "> disconnected.";
	Server::printResponse(oss.str(), RED);
	server.clearClient(client.getFd());
}